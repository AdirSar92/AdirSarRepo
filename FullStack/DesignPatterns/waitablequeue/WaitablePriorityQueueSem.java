/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 31/08/2022
 * @Description: Multithreaded priority queue implementation
 */

package il.co.ilrd.designpatterns.waitablequeue;

import java.time.Duration;
import java.time.LocalTime;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitablePriorityQueueSem<E> {
    private static final int DEFAULT_CAPACITY = 16;
    private final Queue<E> priorityQ; // blank final
    private final Lock queueLock = new ReentrantLock();
    private final Semaphore enqueueLock; // blank final, depends on capacity
    private final Semaphore dequeueLock = new Semaphore(0);

    public WaitablePriorityQueueSem() {
        this(null, DEFAULT_CAPACITY);
    }

    public WaitablePriorityQueueSem(int maxCapacity) {
        this(null, maxCapacity);
    }

    public WaitablePriorityQueueSem(Comparator<? super E> comparator) {
        this(comparator, DEFAULT_CAPACITY);
    }

    public WaitablePriorityQueueSem(Comparator<? super E> comparator, int capacity) {
        priorityQ = new PriorityQueue<>(comparator);
        enqueueLock = new Semaphore(capacity);
    }

    public void enqueue(E element) {
        enqueueLock.acquireUninterruptibly();
        queueLock.lock();
        try {
            priorityQ.add(element);
        } catch (ClassCastException e) {
            enqueueLock.release();
            throw new NotComparableElementException();
        } finally {
            queueLock.unlock();
        }
        dequeueLock.release();
    }

    public E dequeue() {
        E nextElement = null;

        dequeueLock.acquireUninterruptibly();
        queueLock.lock();
        nextElement = priorityQ.poll();
        queueLock.unlock();
        enqueueLock.release();

        return nextElement;
    }

    public E dequeue(long timeout, TimeUnit unit) {
        boolean done = false;
        E toReturn = null;
        LocalTime deadline = LocalTime.now().plusNanos(unit.toNanos(timeout));

        while (!done && LocalTime.now().isBefore(deadline)) {
            try {
                done = dequeueLock.tryAcquire(Duration.between(deadline, LocalTime.now()).toNanos(), TimeUnit.NANOSECONDS);
            } catch (InterruptedException ignored) {
                //IGNORED
            }
        }
        if (done) {
            queueLock.lock();
            toReturn = priorityQ.poll();
            enqueueLock.release();
            queueLock.unlock();
        }

        return toReturn;
    }

    public int size() {
        queueLock.lock();
        int queueSize = priorityQ.size();
        queueLock.unlock();
        return queueSize;
    }

    public boolean isEmpty() {
        queueLock.lock();
        boolean isEmpty = priorityQ.isEmpty();
        queueLock.unlock();
        return isEmpty;
    }

    public boolean remove(E elementToRemove) {
        boolean hasRemoved = false;
        queueLock.lock();
        if (priorityQ.remove(elementToRemove)) {
            enqueueLock.release();
            dequeueLock.acquireUninterruptibly();
            hasRemoved = true;
        }
        queueLock.unlock();
        return hasRemoved;
    }

    public E peek() {
        queueLock.lock();
        E queuePeek = priorityQ.peek();
        queueLock.unlock();
        return queuePeek;
    }

    public static class NotComparableElementException extends RuntimeException {
        public NotComparableElementException() {
            super("Element is not comparable, cannot enqueue");
        }
    }
}
