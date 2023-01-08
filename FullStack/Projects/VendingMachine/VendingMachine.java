/**
 * @Author: Asaf Madari
 * @Reviewer: Omer Desezar
 * @Date: 03.08.22
 * @Description: Vending machine implemented as Finite state machine
 */

package il.co.ilrd.vendingmachine;

import java.time.LocalTime;
import java.util.*;

import static java.lang.Thread.currentThread;

public class VendingMachine {
    private Product currProduct = null;
    private double currMoney = 0;
    private VendingMachineState currState = VendingMachineState.OFF;
    private final Map<Integer, Product> catalog;
    private final Printable printer;
    private Thread timeoutThread = null;
    private final TimeoutCounter timeoutCounter;

    public VendingMachine(Printable printer, Map<Integer, Product> catalog) {
        this.catalog = catalog;
        this.printer = printer;
        this.timeoutCounter = new TimeoutCounter(this);
    }

    public void insertCoin(Coin amount) {
        currState.insertCoin(this, amount);
    }

    public void chooseProduct(Integer product) {
        currState.chooseProduct(this, product);
    }

    public void cancelPurchase() {
        currState.cancelPurchase(this);
    }

    public void LaunchMachine() {
        this.currState.TurnOn(this);
    }

    private void startTimeCount() {
        if (null == timeoutThread) {
            timeoutThread = new Thread(timeoutCounter);
            timeoutThread.start();
        }
        timeoutCounter.timeReset();
    }

    private void killThread() {
        if (null != timeoutThread) {
            timeoutThread.interrupt();
            timeoutThread = null;
        }
    }

    private void returnChange() {
        this.currMoney -= this.currProduct.getPrice();
        this.printer.print("Change returned: " + this.currMoney);
        this.currMoney = 0;
    }

    private double getCurrMoney() {
        return currMoney;
    }

    private enum VendingMachineState {
        OFF {
            public void TurnOn(VendingMachine vendingMachine) {
                vendingMachine.currState = VendingMachineState.WAIT_FOR_PRODUCT_CHOICE;
                System.out.println("Machine is now ON");
            }

            public void insertCoin(VendingMachine vendingMachine, Coin amount) {
                vendingMachine.printer.print("Machine is not running, money not entered");
            }

            public void chooseProduct(VendingMachine vendingMachine, Integer product) {
                vendingMachine.printer.print("Machine is not running");
            }
        },

        WAIT_FOR_PRODUCT_CHOICE {
            public void TurnOn(VendingMachine vendingMachine) {
            }

            public void chooseProduct(VendingMachine vendingMachine, Integer product) {
                vendingMachine.startTimeCount();
                vendingMachine.currProduct = vendingMachine.catalog.get(product);
                vendingMachine.printer.print("Product chosen: " + vendingMachine.currProduct +
                        " Price: " + vendingMachine.currProduct.getPrice());
                vendingMachine.currState = VendingMachineState.WAIT_FOR_PAYMENT;
                checkSufficientFundsForPurchase(vendingMachine);
            }

            public void insertCoin(VendingMachine vendingMachine, Coin amount) {
                vendingMachine.startTimeCount();
                vendingMachine.currMoney += amount.getValue();
                vendingMachine.printer.print("Current balance: " + vendingMachine.getCurrMoney() + " NIS");
                checkSufficientFundsForPurchase(vendingMachine);
            }
        },

        WAIT_FOR_PAYMENT {
            public void TurnOn(VendingMachine vendingMachine) {
            }

            public void chooseProduct(VendingMachine vendingMachine, Integer product) {
                vendingMachine.startTimeCount();
                vendingMachine.currProduct = vendingMachine.catalog.get(product);
                vendingMachine.printer.print("Product chosen: " + vendingMachine.currProduct +
                        " Price: " + vendingMachine.currProduct.getPrice());
                checkSufficientFundsForPurchase(vendingMachine);
            }

            public void insertCoin(VendingMachine vendingMachine, Coin amount) {
                vendingMachine.startTimeCount();
                vendingMachine.currMoney += amount.getValue();
                vendingMachine.printer.print("Current balance: " + vendingMachine.getCurrMoney() + " NIS");
                checkSufficientFundsForPurchase(vendingMachine);
            }
        };

        public abstract void insertCoin(VendingMachine vendingMachine, Coin amount);

        public abstract void chooseProduct(VendingMachine vendingMachine, Integer product);

        public abstract void TurnOn(VendingMachine vendingMachine);

        private void cancelPurchase(VendingMachine vendingMachine) {
            vendingMachine.killThread();
            vendingMachine.currProduct = null;
            vendingMachine.currState = WAIT_FOR_PRODUCT_CHOICE;
            synchronized (this) {
                if (0 != vendingMachine.getCurrMoney()) {
                    vendingMachine.printer.print("Purchase cancelled, returning change: " + vendingMachine.currMoney);
                    vendingMachine.currMoney = 0;
                }
            }
        }

        private static void checkSufficientFundsForPurchase(VendingMachine vendingMachine) {
            if (null != vendingMachine.currProduct &&
                    vendingMachine.getCurrMoney() >= vendingMachine.currProduct.getPrice()) {
                vendProduct(vendingMachine);
            } else {
                vendingMachine.currState = WAIT_FOR_PRODUCT_CHOICE;
            }
        }

        private static void vendProduct(VendingMachine vendingMachine) {
            vendingMachine.printer.print("Vending product: " + vendingMachine.currProduct);
            vendingMachine.returnChange();
            vendingMachine.currProduct = null;
            vendingMachine.currState = WAIT_FOR_PRODUCT_CHOICE;
        }
    }

    private class TimeoutCounter implements Runnable {

        private LocalTime now;
        private final VendingMachine vendingMachine;

        private TimeoutCounter(VendingMachine vendingMachine) {
            this.now = LocalTime.now();
            this.vendingMachine = vendingMachine;
        }

        @Override
        public void run() {
            while (!currentThread().isInterrupted()) {
                if (LocalTime.now().minusSeconds(10).isAfter(now)) {
                    vendingMachine.printer.print("Machine timed out");
                    vendingMachine.cancelPurchase();
                    break;
                }
            }
        }

        private void timeReset() {
            this.now = LocalTime.now();
        }
    }
}
