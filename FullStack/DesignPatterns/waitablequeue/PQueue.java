/**
 * @Author: Adir Sarussi
 * @Reviewer: Tzach Halfon
 * @Date: 31/08/2022
 * @Description: Multithreaded priority queue implementation
 */

package il.co.ilrd.designpatterns.waitablequeue;

import java.util.concurrent.TimeUnit;

/**
 * A collection designed for holding elements prior to processing in a concurrent environment.
 * Besides basic Collection operations, queues provide additional insertion, extraction, and inspection operations.
 * This interface is unconditionally synchronized; no external synchronization is needed
 * dequeue method has two versions:
 * one blocks the current thread until the dequeue succeeds,
 * the other has a timeout parameter (which accepts timeout in seconds).
 * in most implementations, insert operations cannot fail.
 * @param <E> the type of object to inhibit the queue.
 */
public interface PQueue<E> {

    /**
     * inserts the specified element into this queue if it is possible to do so immediately,
     * without violating capacity restrictions,
     * waiting until success to enqueue,
     * and throwing an IllegalStateException if no space is currently available.
     * @param element - the element to add
     * @throws NullPointerException - if the specified element is null and this queue does not permit null elements
     * @throws IllegalArgumentException - if some property of this element prevents it from being added to this queue
     */
    void enqueue(E element);

    /**
     * Retrieves and removes the head of this queue, or returns null if this queue is empty.
     * @return the head of this queue, or null if this queue is empty
     *
     */
    E dequeue();

    /**
     * Retrieves and removes the head of this queue, or returns null if this queue is empty.
     *
     * @param timeout max time to wait for dequeue from the queue, measured in seconds.
     * @param unit time unit for timeout.
     * @return the head of this queue, or null if this queue is empty.
     */
    E dequeue(long timeout, TimeUnit unit);

    /**
     * Checks for this queue's size.
     * @return size of current queue.
     */
    int size();

    /**
     * Checks if the current queue is empty or not.
     * @return true if queue is empty, else false.
     */
    boolean isEmpty();

    /**
     * Removes a requested element from the queue.
     * @param element the element to remove.
     */
    void remove(E element);

    /**
     * Retrieves, but does not remove, the head of this queue,
     * or returns null if this queue is empty.
     * @return the head of this queue, or null if this queue is empty
     */
    E peek();
}
