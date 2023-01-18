

package il.co.ilrd.concurrency;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReentrantLock;

public class CostOfLocking {

    private static final int TEN_MILLION_TIMES = 10000000;
    private static final AtomicInteger atomicCounter = new AtomicInteger(0);
    private static int counter = 0;

    public static void main(String[] args) {
        execute(new SynchronizedMethod(), "SynchronizedMethod: ", false);
        execute(new SynchronizedBlock(), "SynchronizedBlock: ", false);
        execute(new SynchronizedAtomic(), "SynchronizedAtomic: ", true);
        execute(new SynchronizedReentrant(), "SynchronizedReentrant: ", false);
    }

    private static void execute(Runnable runnable, String syncType, boolean isAtomic) {
        Thread t1 = new Thread(runnable);
        Thread t2 = new Thread(runnable);

        System.out.println(syncType);
        long start = System.currentTimeMillis();
        t1.start();
        t2.start();
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        long end = System.currentTimeMillis();
        System.out.println("Counter is: " + ((isAtomic) ? atomicCounter : counter));
        System.out.println("Elapsed time : " + (end - start) + "ms");
        counter = 0;
    }

    public static class SynchronizedReentrant implements Runnable {
        ReentrantLock lock = new ReentrantLock();

        @Override
        public void run() {
            lock.lock();
            for (int i = 0; i < TEN_MILLION_TIMES; i++) {
                ++counter;
            }
            lock.unlock();
        }
    }

    //very expensive! but, good on minimizing critical section
    private static class SynchronizedAtomic implements Runnable {
        @Override
        public void run() {
            for (int i = 0; i < TEN_MILLION_TIMES; i++) {
                CostOfLocking.atomicCounter.incrementAndGet();
            }
        }
    }

    private static class SynchronizedBlock implements Runnable {
        @Override
        public void run() {
            synchronized (this) {
                for (int i = 0; i < TEN_MILLION_TIMES; i++) {
                    ++counter;
                }
            }
        }
    }

    private static class SynchronizedMethod implements Runnable {
        @Override
        public synchronized void run() {
            for (int i = 0; i < TEN_MILLION_TIMES; i++) {
                ++counter;
            }
        }
    }
}

