/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 23/08/2022
 * @Description:
 */

package il.co.ilrd.concurrency;

import java.time.LocalTime;
import java.util.LinkedList;
import java.util.List;

public class ProducerConsumerListSync {

    private static final int NUM_OF_PARTICIPANTS = 5;

    public static void main(String[] args) {

        Thread[] producers = new Thread[NUM_OF_PARTICIPANTS];
        Thread[] consumers = new Thread[NUM_OF_PARTICIPANTS];

        for (int i = 0; i < NUM_OF_PARTICIPANTS; i++) {
            producers[i] = new Thread(new Producer());
            consumers[i] = new Thread(new Consumer());
        }

        for (int i = 0; i < NUM_OF_PARTICIPANTS; i++) {
            consumers[i].start();
            producers[i].start();
        }
    }

    private static final List<Integer> list = new LinkedList<>();
    private static final LocalTime now = LocalTime.now();

    private static class Producer implements Runnable {
        @Override
        public void run() {
            while (!(LocalTime.now().minusSeconds(5).isAfter(now))) {
                synchronized (list) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    int temp = (int) (Math.random() * 100);
                    list.add(temp);
                    System.out.println(Thread.currentThread().getName() +  " Producer adds: " + temp);
                }
            }
        }
    }

    private static class Consumer implements Runnable {

        @Override
        public void run() {
            while (!(LocalTime.now().minusSeconds(5).isAfter(now))) {
                synchronized (list) {
                    if (!list.isEmpty()) {
                        try {
                            Thread.sleep(1000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        int temp = list.get(0);
                        System.out.println(Thread.currentThread().getName() + " Consumer gets: " + temp);
                        list.remove(0);
                    }
                }
            }
        }
    }
}
