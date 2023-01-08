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
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitablePriorityQueue<E> {
    private final Queue<E> priorityQ; // blank final
    private final Lock gate = new ReentrantLock();
    private final Condition dequeueLock = gate.newCondition();

    public WaitablePriorityQueue() {
        this(null);
    }

    public WaitablePriorityQueue(Comparator<? super E> comparator) {
        priorityQ = new PriorityQueue<>(comparator);
    }

    public void enqueue(E element) {
        gate.lock();
        try {
            priorityQ.add(element);
        } catch (ClassCastException e) {
            throw new NotComparableElementException();
        }
        dequeueLock.signalAll();
        gate.unlock();
    }

    public E dequeue() {
        E nextElement = null;
        gate.lock();
        while (priorityQ.isEmpty()) {
            dequeueLock.awaitUninterruptibly();
        }
        nextElement = priorityQ.poll();
        gate.unlock();
        return nextElement;
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    public E dequeue(long timeout, TimeUnit unit) {
        E toReturn = null;
        LocalTime deadline = LocalTime.now().plusNanos(unit.toNanos(timeout));

        gate.lock();
        while (priorityQ.isEmpty() && LocalTime.now().isBefore(deadline)) {
            try {
                dequeueLock.await(Duration.between(deadline, LocalTime.now()).toNanos(), TimeUnit.NANOSECONDS);
            } catch (InterruptedException ignored) {
                //IGNORED
            }
        }
        toReturn = priorityQ.poll();
        gate.unlock();
        return toReturn;
    }

    public int size() {
        gate.lock();
        int queueSize = priorityQ.size();
        gate.unlock();
        return queueSize;
    }

    public boolean isEmpty() {
        gate.lock();
        boolean isEmpty = priorityQ.isEmpty();
        gate.unlock();
        return isEmpty;
    }

    public boolean remove(E elementToRemove) {
        gate.lock();
        boolean hasRemoved = priorityQ.remove(elementToRemove);
        gate.unlock();
        return hasRemoved;
    }

    public E peek() {
        gate.lock();
        E queuePeek = priorityQ.peek();
        gate.unlock();
        return queuePeek;
    }

    public static class NotComparableElementException extends RuntimeException {
        public NotComparableElementException() {
            super("Element is not comparable, cannot enqueue");
        }
    }
}
