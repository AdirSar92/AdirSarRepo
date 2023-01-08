package il.co.ilrd.threadpool;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.*;

public class FlowTester {

    private static final ThreadPool threadPool = new ThreadPool();

    public static void main(String[] args) throws InterruptedException {
        System.out.println(" >>> basic tests, not regarding futures <<< ");
        addTasks(16, 1, null);
        Thread.sleep(4050);
        System.out.println(" ~~ setting num of threads to 1 ~~ ");
        threadPool.setNumOfThreads(1);
        addTasks(4, 1, null);
        Thread.sleep(4050);
        System.out.println(" ~~ setting num of threads to 20 ~~ ");
        threadPool.setNumOfThreads(20);
        addTasks(80, 1, null);
        Thread.sleep(4050);
        System.out.println(" ~~ setting num of threads to 4 ~~ ");
        threadPool.setNumOfThreads(4);
        addTasks(16, 1, null);
        Thread.sleep(1950);
        System.out.println(" ~~ pausing ~~ ");
        threadPool.pause();
        Thread.sleep(4050);
        System.out.println(" ~~ resuming ~~ ");
        threadPool.resume();
        Thread.sleep(2050);

        List<Future<Integer>> futures = new ArrayList<>();
        System.out.println(" >>> advanced tests <<< ");
        addTasks(12, 4, futures);
        Thread.sleep(5300);

        System.out.println("> canceling a task that is done: " + futures.get(0).cancel(true));
        System.out.println("> canceling a task that is executing: " + futures.get(4).cancel(true));
        System.out.println("> canceling a task that has not started yet: " + futures.get(8).cancel(true));

        try {
            System.out.println("> result from cancelled done task: " + futures.get(0).get());
        } catch (ExecutionException e) {
            e.printStackTrace();
        }

        System.out.println("> getting result from cancelled executing task, should throw CancellationException");
        try {
            futures.get(4).get();
        } catch (ExecutionException | CancellationException e ) {
            e.printStackTrace();
        }

        System.out.println("> getting result from cancelled not done task, should throw CancellationException");
        try {
            futures.get(8).get();
        } catch (ExecutionException | CancellationException e) {
            e.printStackTrace();
        }

        Thread.sleep(7000);
        futures.clear();
        addTasks(2, 5, futures);
        System.out.println("> getting result with not enough time, should throw TimeoutException");
        try {
            System.out.println(futures.get(0).get(1500, TimeUnit.MILLISECONDS));
        } catch (TimeoutException | ExecutionException e) {
            e.printStackTrace();
        }
        try {
            System.out.println("> getting result with enough time: " + futures.get(0).get(6, TimeUnit.SECONDS));
        } catch (TimeoutException | ExecutionException e) {
            e.printStackTrace();
        }

        futures.clear();
        addTasks(5, 4, futures);
        System.out.println(" ~~ setting num of threads to 2, then 1 immediately ~~ ");
        threadPool.setNumOfThreads(2);
        threadPool.setNumOfThreads(1);
        try {
            System.out.println("> result from last task: " + futures.get(4).get());
        } catch (ExecutionException e) {
            throw new RuntimeException(e);
        }
        System.out.println(" ~~ setting num of threads to 100 ~~ ");
        threadPool.setNumOfThreads(100);
        addTasks(500, 2, null);
        System.out.println(" ~~ shutting down ~~ ");
        threadPool.shutdown();
        threadPool.awaitTermination();
        System.out.println(">>> END OF PROGRAM <<<");
    }

    private static void addTasks(int numOfTasks, int numOfLoops, List<Future<Integer>> futures) throws InterruptedException {
        System.out.println(" ~~ adding " + numOfTasks + " tasks to the threadpool ~~ ");
        for (int i = 1; i <= numOfTasks; i++) {
            Future<Integer> future = threadPool.submit(new GenericTask(numOfLoops, i));
            if (null != futures) futures.add(future);
            Thread.sleep(1);
        }
    }

    private static class GenericTask implements Callable<Integer> {
        private final int loops;
        private final int taskIndex;
        private final Random rand = new Random();

        public GenericTask(int loops, int taskIndex) {
            this.loops = loops;
            this.taskIndex = taskIndex;
        }

        @Override
        public Integer call() {
            for (int i = 1; i <= loops && !Thread.currentThread().isInterrupted(); i++) {
                System.out.println("Task " + taskIndex + " : " + Thread.currentThread().getName() + " in loop " + i + " out of " + loops);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    System.out.println("Task " + taskIndex + " : " + Thread.currentThread().getName() + " interrupted");
                    return 0;
                }
            }
            return rand.nextInt(100);
        }
    }
}
