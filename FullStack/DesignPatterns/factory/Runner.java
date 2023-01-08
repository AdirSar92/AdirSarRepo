/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description:
 */

package il.co.ilrd.designpatterns.factory;

public class Runner {
    public static void main(String[] args) {
        Factory<Integer, Product, String> factory = new Factory<>();

        factory.add(1, ProductA::new);
        factory.add(2, (str) -> new ProductB(str));
        factory.add(3, ProductA::foo);

        ProductA productA = (ProductA) factory.createProduct(1 ,"Product A says Hello");
        Product productB = factory.createProduct(2 ,"Product B says Hello");

        productA.doStuff();
        productB.doStuff();
        factory.add(4, productA::bar);
    }
}
