/**
 * @Author: Asaf Madari
 * @Reviewer: Omer Desezar
 * @Date: 03.08.22
 * @Description: Vending machine implemented as Finite state machine
 */

package il.co.ilrd.vendingmachine;

public enum Product {
    BAMBA("Bamba", 3.40),
    BISLI("Bisli", 4.20),
    KLIK("Red Klik", 6.20),
    KINDER("Kinder Bueno", 7.50),
    COLA("Coca Cola", 4),
    SPRITE("Sprite", 4.10),
    FANTA("Fanta", 4.40),
    WATER("Water", 3.80),
    SODA("Soda", 3.90),
    BEER("Beer", 18.50);

    private final String productName;
    private final double price;

    private Product(String productName, double price) {
        this.productName = productName;
        this.price = price;
    }

    public String getProductName() {
        return productName;
    }

    public double getPrice() {
        return price;
    }
}
