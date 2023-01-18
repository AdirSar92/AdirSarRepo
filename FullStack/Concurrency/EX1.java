

package il.co.ilrd.concurrency;

public class EX1 {

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(new RunnableIMP());
        t1.start();
        System.out.println("Hello from main: Thread 1");

        SubThread t2 = new SubThread();
        t2.start();

        System.out.println("Main Sleeping for 1 sec before interrupting thread 2");
        Thread.sleep(1000);
        System.out.println("Interrupting thread 2");

        t2.interrupt();
    }

    private static class RunnableIMP implements Runnable {
        @Override
        public void run() {
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Hello from thread 1");
        }
    }

    private static class SubThread extends Thread {
        @Override
        public void run() {
            while (true) {
                if (isInterrupted()){
                    System.out.println("Thread 2 Interrupted");
                    break;
                }
            }
        }
    }
}
