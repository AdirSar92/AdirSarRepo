package il.co.ilrd.designpatterns.waitablequeue;

import il.co.ilrd.Colour;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.TimeUnit;

import static java.lang.Thread.sleep;

public class CondPQueueRunner {
    private static final WaitablePriorityQueue<PriorityObject> pq = new WaitablePriorityQueue<>();
    private static final int NUM_OF_THREADS = 5;

    public static void main(String[] args) {
        Runnable producerRunnable = new ProducerRunnable();
        Runnable consumerRunnable = new ConsumerRunnable();
        Runnable consumerTimeout = new ConsumerTimeout();
        Runnable sideChecks = new SideChecks();

        List<Thread> producers = new ArrayList<>();
        List<Thread> consumers = new ArrayList<>();
        Thread timeoutThread = new Thread(consumerTimeout);
        Thread sideChecker = new Thread(sideChecks);

        for (int i = 0; i < NUM_OF_THREADS; i++) {
            producers.add(new Thread(producerRunnable));
            consumers.add(new Thread(consumerRunnable));
        }

        producers.get(0).start();
        producers.get(1).start();
        producers.get(2).start();
        producers.get(3).start();
        producers.get(4).start();
        timeoutThread.start();
        sideChecker.start();

        consumers.get(0).start();
        consumers.get(1).start();
    }

    private static class ProducerRunnable implements Runnable {
        @Override
        public void run() {
            int i = 0;
            while (true) {
                ++i;
                int rand = Math.abs(new Random().nextInt()) % 50;
                PriorityObject priorityObject = new PriorityObject(rand);
                Colour.print(Colour.WHITE_BG, Thread.currentThread().getName() + " produced " + rand);
                pq.enqueue(priorityObject);
                try {
                    sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                if (0 == i % 100) pq.remove(priorityObject);
            }
        }
    }

    private static class ConsumerRunnable implements Runnable {
        @Override
        public void run() {
            while (true) {
                Colour.print(Colour.RED_BG, Thread.currentThread().getName() + " consumes " + pq.dequeue().getPriority());
                try {
                    sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static class ConsumerTimeout implements Runnable {
        @Override
        public void run() {
            PriorityObject deq;
            while (true) {
                deq = pq.dequeue(2, TimeUnit.SECONDS);
                Colour.print(Colour.PURPLE_BG, Thread.currentThread().getName() + " timed consumes " + deq.getPriority());
            }
        }
    }

    private static class SideChecks implements Runnable {
        @Override
        public void run() {
            while (true) {
                PriorityObject peek = pq.peek();
                Colour.print(Colour.GREEN_FG, "current peek is: " + ((null != peek) ? peek.getPriority() : null));
                Colour.print(Colour.ORANGE_FG , "current size is: " + pq.size());
                Colour.print(Colour.CYAN_FG, "current list is: " + (pq.isEmpty() ? "empty" : "not empty"));
                try {
                    sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static class PriorityObject implements Comparable<PriorityObject> {

        private static final int DEFAULT_PRIORITY = 10;
        private final int priority; //blank final

        public PriorityObject() {
            this(DEFAULT_PRIORITY);
        }
        public PriorityObject(int priority) {
            this.priority = priority;
        }

        public int getPriority() {
            return priority;
        }

        @Override
        public int compareTo(PriorityObject other) {
            return this.priority - other.priority;
        }
    }
}


