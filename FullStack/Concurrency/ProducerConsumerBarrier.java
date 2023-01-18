
package il.co.ilrd.concurrency;

import java.util.concurrent.Executor;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ProducerConsumerBarrier {
    private static final int NUM_OF_PARTICIPANTS = 5;
    private static final Semaphore sem = new Semaphore(0);
    private static int globalData = 0;
    private static final Lock lock = new ReentrantLock();
    private static final Condition condition = lock.newCondition();

    public static void main(String[] args) {

        Thread producer = new Thread(new Producer());
        Thread[] consumers = new Thread[NUM_OF_PARTICIPANTS];

        for (int i = 0; i < NUM_OF_PARTICIPANTS; i++) {
            consumers[i] = new Thread(new Consumer());
        }

        for (int i = 0; i < NUM_OF_PARTICIPANTS; i++) {
            consumers[i].start();
        }
        producer.start();
    }

    private static class Producer implements Runnable {
        @Override
        public void run() {
            while (true) {
                int localData = produce();
                if (localData > 100) break;

                try {
                    sem.acquire(5);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                lock.lock();
                globalData = localData;
                condition.signalAll();
                System.out.println("Producer sending message");
                lock.unlock();
            }
        }

        private static int produce() {
            ++globalData;
            return globalData;
        }
    }

    private static class Consumer implements Runnable {

        @Override
        public void run() {
            int localData = 0;
            while (true) {
                try {
                    if (globalData > 100) {
                        lock.lock();
                        condition.signalAll();
                        lock.unlock();
                        break;
                    }
                    lock.lock();
                    sem.release();
                    condition.await();
                    localData = globalData;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                lock.unlock();
                consume(localData);
            }
        }

        private static void consume(int localData) {
            System.out.println(Thread.currentThread().getName() + " says: " + localData);
        }
    }
}


