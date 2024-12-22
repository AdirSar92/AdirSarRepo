package il.co.ilrd.designpatterns.waitablequeue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.TimeUnit;

import static org.junit.jupiter.api.Assertions.*;

class SemPQueueTest {

    private WaitablePriorityQueueSem<Integer> queue;
    private List<Thread> producers;
    private List<Thread> consumers;
    private List<Thread> timedConsumers;

    @BeforeEach
    void init() {
        queue = new WaitablePriorityQueueSem<>(10);
        queue.enqueue(2);
        queue.enqueue(8);
        queue.enqueue(4);
    }

    @Test
    void enqueue() {
        assertEquals(3, queue.size());
        queue.enqueue(1);
        queue.enqueue(2);
        assertEquals(5, queue.size());
    }

    @Test
    void dequeue() {
        assertEquals(3, queue.size());
        assertEquals(2, queue.dequeue());
        assertEquals(4, queue.dequeue());
        assertEquals(1, queue.size());

    }

    @Test
    void timedDequeue() {
        queue.dequeue();
        queue.dequeue();
        assertNotNull(queue.dequeue(1, TimeUnit.SECONDS));
        assertNull(queue.dequeue(1, TimeUnit.SECONDS));
    }

    @Test
    void peek() {
        assertEquals(2, queue.peek());
        queue.dequeue();
        assertEquals(4, queue.peek());
    }

    @Test
    void size() {
        assertEquals(3, queue.size());
        queue.enqueue(2);
        queue.enqueue(3);
        queue.enqueue(4);
        assertEquals(6, queue.size());
        queue.dequeue();
        queue.dequeue();
        assertEquals(4, queue.size());
    }

    @Test
    void isEmpty() {
        assertFalse(queue.isEmpty());
        queue.dequeue();
        queue.dequeue();
        queue.dequeue();
        assertTrue(queue.isEmpty());
    }

    @Test
    void remove() {
        queue.remove(5);
        assertEquals(3, queue.size());
        queue.remove(8);
        assertEquals(2, queue.size());
    }

    @Test
    void mtTest1() {
        System.out.println("Test1 - producers > consumers. expected output: NONE");
        initThreads();
        producers.get(0).start();
        producers.get(1).start();
        producers.get(2).start();
        producers.get(3).start();
        consumers.get(0).start();
        consumers.get(1).start();
        timedInterruptThreads();
    }

    @Test
    void mtTest2() {
        System.out.println("Test2 - consumers > producers. expected output: NONE");
        initThreads();
        producers.get(0).start();
        producers.get(1).start();
        consumers.get(0).start();
        consumers.get(1).start();
        consumers.get(2).start();
        consumers.get(3).start();
        timedInterruptThreads();
    }

    @Test
    void mtTest3() {
        System.out.println("Test3 - producers > timed consumers. expected output: NONE (possible output: TimedOut)");
        initThreads();
        producers.get(0).start();
        producers.get(1).start();
        producers.get(2).start();
        producers.get(3).start();
        timedConsumers.get(0).start();
        timedConsumers.get(1).start();
        timedInterruptThreads();
    }

    @Test
    void mtTest4() {
        System.out.println("Test4 - timed consumers > producers. expected output: TIMEOUT");
        initThreads();
        producers.get(0).start();
        producers.get(1).start();
        timedConsumers.get(0).start();
        timedConsumers.get(1).start();
        timedConsumers.get(2).start();
        timedConsumers.get(3).start();
        timedInterruptThreads();
    }

    private void initThreads() {
        producers = new ArrayList<>();
        consumers = new ArrayList<>();
        timedConsumers = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            producers.add(new Thread(new Producer(queue)));
            consumers.add(new Thread(new Consumer(queue)));
            timedConsumers.add(new Thread(new TimeConsumer(queue)));
        }
    }

    private void timedInterruptThreads() {
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        for (int i = 0; i < 4; i++) {
            producers.get(i).interrupt();
            consumers.get(i).interrupt();
            timedConsumers.get(i).interrupt();
        }
    }

    static class Producer implements Runnable {

        private final WaitablePriorityQueueSem<Integer> q;

        public Producer(WaitablePriorityQueueSem<Integer> q) {
            this.q = q;
        }

        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                q.enqueue((Math.abs(new Random().nextInt()) % 100) + 1);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException ignored) {/*IGNORED*/}
            }
        }
    }

    static class Consumer implements Runnable {

        private final WaitablePriorityQueueSem<Integer> q;

        public Consumer(WaitablePriorityQueueSem<Integer> q) {
            this.q = q;
        }

        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                if (null == q.dequeue()) System.out.println("NULL -> UnExpected");
            }
        }
    }

    static class TimeConsumer implements Runnable {

        private final WaitablePriorityQueueSem<Integer> q;

        public TimeConsumer(WaitablePriorityQueueSem<Integer> q) {
            this.q = q;
        }

        @Override
        public void run() {
            while (!Thread.currentThread().isInterrupted()) {
                if (null == q.dequeue(1, TimeUnit.SECONDS)) System.out.println("TimedOut");
            }
        }
    }

}
