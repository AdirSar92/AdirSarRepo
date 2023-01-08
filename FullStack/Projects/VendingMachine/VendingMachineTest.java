/**
 * @Author: Asaf Madari
 * @Reviewer: Omer Desezar
 * @Date: 03.08.22
 * @Description: Vending machine implemented as Finite state machine
 */

package il.co.ilrd.vendingmachine;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

class VendingMachineTest {
    static Scanner scanner = new Scanner(System.in);
    static Map<Integer, Product> inventory = new HashMap<>();
    static VendingMachine machine = null;

    public static void main(String[] args) {

        for (Product product :
                Product.values()) {
            inventory.put(product.ordinal(), product);
        }
        machine = new VendingMachine(new Printer(), inventory);
        machine.LaunchMachine();

        while (true) {
            VendingMachineTest.printActionMenu();
            int userRequest = scanner.nextInt();
            if (5 == userRequest) {
                machine.cancelPurchase();
                break;
            }
            performUserRequest(userRequest, machine);
        }

    }

    private static void printActionMenu(){
        System.out.println("Please choose action:");
        System.out.println("1 : Insert Coins");
        System.out.println("2 : Choose Product");
        System.out.println("3 : Cancel Purchase");
        System.out.println("4 : Print Menu");
        System.out.println("5 : Exit");
    }

    private static void menuPrint(){
        System.out.println("Vending machine inventory: ");
        System.out.println("Code Name         Price");
        inventory.forEach((productNumber, product) -> System.out.format("%-4d %-12s %2s NIS\n", productNumber,
                                                        product.getProductName(), product.getPrice()));
    }

    private static void performUserRequest(int action, VendingMachine vendingMachine) {
        switch (action) {
            case 1:
                getCoinToInsert(vendingMachine);
                break;
            case 2:
                getProductChoice(vendingMachine);
                break;
            case 3:
                vendingMachine.cancelPurchase();
                break;
            case 4:
                menuPrint();
        }
    }

    private static void getCoinToInsert(VendingMachine vendingMachine){
        System.out.print("Please Choose coin: ");
        Coin coin = Coin.numberToCoin(scanner.nextDouble());
        while (null == coin) {
            System.out.println("Invalid coin, Please try again");
            coin = Coin.numberToCoin(scanner.nextDouble());
        }
        vendingMachine.insertCoin(coin);
    }

    private static void getProductChoice(VendingMachine vendingMachine){
        System.out.print("Please enter product code: ");
        vendingMachine.chooseProduct(scanner.nextInt());
    }


}