/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 23/08/2022
 * @Description:
 */

package il.co.ilrd.concurrency;

import java.time.LocalTime;
import java.util.concurrent.Semaphore;

public class ProducerConsumerSemaphore {

    public static void main(String[] args) {
        Thread producer = new Thread(new Producer());
        Thread consumer = new Thread(new Consumer());
        producer.start();
        consumer.start();
    }

    private static final Semaphore pingSemaphore = new Semaphore(1);
    private static final Semaphore pongSemaphore = new Semaphore(0);
    private static final LocalTime now = LocalTime.now();

    private static class Producer implements Runnable {

        @Override
        public void run() {
            while (!(LocalTime.now().minusSeconds(7).isAfter(now))) {
                try {
                    pingSemaphore.acquire();
                    Thread.sleep(1000);
                    Ping();
                    pongSemaphore.release();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static class Consumer implements Runnable {

        @Override
        public void run() {
            while (!(LocalTime.now().minusSeconds(5).isAfter(now))) {
                try {
                    pongSemaphore.acquire();
                    Thread.sleep(1000);
                    Pong();
                    pingSemaphore.release();
                } catch (InterruptedException e) {
                    e.printStackTrace();
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

