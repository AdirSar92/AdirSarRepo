/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 23/08/2022
 * @Description:
 */

package il.co.ilrd.concurrency;

import java.time.LocalTime;
import java.util.concurrent.atomic.AtomicBoolean;

public class ProducerConsumerAtomic {

    private static final AtomicBoolean flag = new AtomicBoolean(true);
    private static final LocalTime now = LocalTime.now();

    public static void main(String[] args) {
        Thread producer = new Thread(new Producer());
        Thread consumer = new Thread(new Consumer());
        producer.start();
        consumer.start();
    }

    private static class Producer implements Runnable {
        @Override
        public void run() {
           while (!(LocalTime.now().minusSeconds(7).isAfter(now))) {
               if (flag.get()) {
                   try {
                       Thread.sleep(1000);
                   } catch (InterruptedException e) {
                        e.printStackTrace();
                   }
                   System.out.println(Thread.currentThread().getName() + " Says: ");
                   Ping();
                   flag.set(false);
               }
           }
        }
    }

    private static class Consumer implements Runnable {
        @Override
        public void run() {
            while (!(LocalTime.now().minusSeconds(7).isAfter(now))) {
                if (!flag.get()) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " Says: ");
                    Pong();
                    flag.set(true);
                }
            }
        }
    }

    private static void Ping() {
        System.out.println("Ping");
    }


    private static void Pong() {
        System.out.println("Pong");
    }
}
