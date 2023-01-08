/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description:
 */

package il.co.ilrd.designpatterns.factory;

public class ProductB implements Product {
    private final String str;

    public ProductB(String str) {
        this.str = str;
    }

    @Override
    public void doStuff() {
        System.out.println(str);
    }
}
