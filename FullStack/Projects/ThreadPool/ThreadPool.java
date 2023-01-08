/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 07/09/2022
 * @Description: Thread Pool implementation
 */

package il.co.ilrd.threadpool;

import il.co.ilrd.designpatterns.waitablequeue.WaitablePriorityQueue;

import java.time.Duration;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.*;

public class ThreadPool implements Executor {

    private static final int DEFAULT_NUM_OF_THREADS = Runtime.getRuntime().availableProcessors();
    private static final int LOWEST_PRIORITY = Priority.LOW.ordinal() - 1;
    private static final int HIGHEST_PRIORITY = Priority.HIGH.ordinal() + 1;
    private static final Runnable POISON_PILL = () -> ((ThreadIMP) Thread.currentThread()).isActive = false;

    private final List<Thread> threads = new LinkedList<>(); //o(n) remove, o(1) add
    private final WaitablePriorityQueue<Task<?>> taskQueue = new WaitablePriorityQueue<>();
    private final Semaphore pauseThread = new Semaphore(0);
    private final Runnable SLEEPING_PILL = pauseThread::acquireUninterruptibly;

    private volatile boolean isRunning = true;
    private volatile boolean isPaused = false;
    private int currentActiveWorkers = 0;
    private int pausedActiveThreads;

    public ThreadPool() {
        this(DEFAULT_NUM_OF_THREADS);
    }

    public ThreadPool(int numOfThreads) {
        if (numOfThreads < 0) throw new IllegalArgumentException();
        currentActiveWorkers = numOfThreads;
        addThreads(numOfThreads);
    }

    //inner class
    public enum Priority {
        LOW, NORMAL, HIGH
    }

    @Override
    public void execute(Runnable task) {
        submit(Executors.callable(task), Priority.NORMAL);
    }

    public <V> Future<V> submit(Callable<V> task, Priority priority) {
        if (isRunning) {
            Task<V> newTask = new Task<>(task, priority.ordinal());
            taskQueue.enqueue(newTask);
            return newTask.future;
        }
        throw new IllegalStateException();
    }

    public <V> Future<V> submit(Callable<V> task) {
        return submit(task, Priority.NORMAL);
    }

    public <V> Future<V> submit(Runnable task, V returnValue, Priority priority) {
        return submit(Executors.callable(task, returnValue), priority);
    }

    public Future<?> submit(Runnable task, Priority priority) {
        return submit(Executors.callable(task), priority);
    }

    public void shutdown() {
        shutdown(LOWEST_PRIORITY);
    }

    public List<Task<?>> shutdownNow() {
        List<Task<?>> remainingTasks = getRemainingTasks();
        shutdown(HIGHEST_PRIORITY);
        resume();
        return remainingTasks;

    }

    public void setNumOfThreads(int numOfThreads) {
        if (0 > numOfThreads) throw new IllegalArgumentException();

        if (isRunning) {
            threads.removeIf(thread -> !thread.isAlive());

            if (currentActiveWorkers < numOfThreads) {
                if (isPaused) {
                    addPillToQueue(SLEEPING_PILL, numOfThreads - currentActiveWorkers, HIGHEST_PRIORITY);
                }
                addThreads(numOfThreads - currentActiveWorkers);
            } else {
                addPillToQueue(POISON_PILL, currentActiveWorkers - numOfThreads, HIGHEST_PRIORITY);
            }
            if (!isPaused) {
                currentActiveWorkers = numOfThreads;
            } else {
                pausedActiveThreads = numOfThreads;
            }
        }
    }

    public void awaitTermination() throws InterruptedException {
        if (isRunning) throw new IllegalStateException();
        for (Thread currThread : threads) {
            currThread.join();
        }
    }

    public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
        if (isRunning) throw new IllegalStateException();

        LocalDateTime deadline = LocalDateTime.now().plusNanos(unit.toNanos(timeout));
        for (Thread thread : threads) {
            long currDuration = Duration.between(LocalDateTime.now(), deadline).toMillis();
            if (currDuration < 0) return false;
            thread.join(currDuration);
        }
        return true;
    }

    public void pause() {
        if (!isPaused) {
            isPaused = true;
            addPillToQueue(SLEEPING_PILL, currentActiveWorkers, HIGHEST_PRIORITY);
        }
    }

    public void resume() {
        if (!isPaused) throw new IllegalStateException();
        isPaused = false;
        pauseThread.release(currentActiveWorkers);
        if (currentActiveWorkers != pausedActiveThreads) {
            currentActiveWorkers = pausedActiveThreads;
        }
    }

    private void addThreads(int threadsToAdd) {
        for (int i = 0; i < threadsToAdd; i++) {
            ThreadIMP newThread = new ThreadIMP();
            threads.add(newThread);
            newThread.start();
        }
    }

    private void shutdown(int priority) {
        if (isRunning) {
            isRunning = false;
            addPillToQueue(POISON_PILL, currentActiveWorkers, priority);
        }
    }

    private void addPillToQueue(Runnable pillType, int numOfPills, int priority) {
        for (int i = 0; i < numOfPills; ++i) {
            taskQueue.enqueue(new Task<>(Executors.callable(pillType), priority));
        }
    }

    private List<Task<?>> getRemainingTasks() {
        List<Task<?>> remainingTasks = new ArrayList<>();
        Task<?> dequeuedTask = taskQueue.dequeue(0, TimeUnit.SECONDS);

        while (null != dequeuedTask) {
            remainingTasks.add(dequeuedTask);
            dequeuedTask = taskQueue.dequeue(0, TimeUnit.SECONDS);
        }

        return remainingTasks;
    }

    //inner class
    private class Task<V> implements Comparable<Task<?>> {
        private Thread executorThread = null;
        private final int priority; // blank final
        private final Callable<V> taskCallable; // blank final
        private final TaskFutureIMP future = new TaskFutureIMP();
        private volatile boolean isTaskCancelled = false;
        private final LocalDateTime creationTime = LocalDateTime.now();

        private Task(Callable<V> taskCallable, int priority) {
            this.priority = priority;
            this.taskCallable = taskCallable;
        }

        @Override
        public int compareTo(Task<?> other) {
            int priorityCompare = other.priority - this.priority; //(other - this) is for big->small arrangement
            int timeCompare = this.creationTime.compareTo(other.creationTime); //(this - other) is for old->young arrangement
            return ((0 == priorityCompare) ? timeCompare : priorityCompare);
        }

        private void run() {
            executorThread = Thread.currentThread();
            try {
                future.resultValue = taskCallable.call();
            } catch (Exception e) {
                future.taskRunException = new ExecutionException(e); //assign value for exception thrown from user method
            } finally {
                markTaskDone();
            }
        }

        private void markTaskDone() {
            future.done = true;
            future.taskFinished.release();
        }

        //inner inner class
        private class TaskFutureIMP implements Future<V> {
            private volatile boolean done = false;
            private volatile V resultValue = null;
            private volatile ExecutionException taskRunException = null;
            private final Semaphore taskFinished = new Semaphore(0);

            //default constructor

            @Override
            public boolean cancel(boolean mayInterruptIfRunning) {
                if (done && !isTaskCancelled) {
                    isTaskCancelled = false; //task already finished
                } else if (taskQueue.remove(Task.this)) {
                    resultValue = null;
                    isTaskCancelled = true; //managed to cancel successfully
                } else if (mayInterruptIfRunning) {
                    executorThread.interrupt();
                    isTaskCancelled = true;
                }
                done = true; //from the first time calling cancel, isDone returns true, isCancelled return true if this method returned true.
                return isTaskCancelled;
            }

            @Override
            public boolean isCancelled() {
                return isTaskCancelled;
            }

            @Override
            public boolean isDone() {
                return (done);
            }

            @Override
            public V get() throws InterruptedException, CancellationException, ExecutionException {
                if (isTaskCancelled) throw new CancellationException();
                if (!done) {
                    taskFinished.acquire();
                }
                if (null != taskRunException) throw taskRunException;
                return resultValue;
            }

            @Override
            public V get(long maxTimeout, TimeUnit timeUnit) throws InterruptedException, ExecutionException, TimeoutException {
                if (isTaskCancelled) throw new CancellationException();

                boolean gotPermit = false;

                gotPermit = taskFinished.tryAcquire(maxTimeout, timeUnit);
                if (gotPermit) {
                    done = true;
                    if (null == taskRunException) {
                        return future.resultValue;
                    }
                    throw taskRunException;
                }
                throw new TimeoutException();
            }
        }
    }

    // inner class
    private class ThreadIMP extends Thread {

        private volatile boolean isActive = true;

        @Override
        public void run() {
            Task<?> currTask = null;
            while (isActive) {
                currTask = taskQueue.dequeue();
                currTask.run();
            }
        }
    }
}