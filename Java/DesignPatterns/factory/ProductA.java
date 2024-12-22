/**
 * @Author: Adir Sarussi
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description:
 */

package il.co.ilrd.designpatterns.factory;

public class ProductA implements Product {
    private final String str;

    public ProductA(String str) {
        this.str = str;
    }

    @Override
    public void doStuff() {
        System.out.println(str);
    }
    
    public static Product foo(String a) {
        return new ProductA(a);
    }

    public Product bar(String a) {
        return new ProductA(a);
    }
}
