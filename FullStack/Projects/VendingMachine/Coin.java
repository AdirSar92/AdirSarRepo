/**
 * @Author: Adir Sarussi
 * @Reviewer: Omer Desezar
 * @Date: 03.08.22
 * @Description: Vending machine implemented as Finite state machine
 */

package il.co.ilrd.vendingmachine;

public enum Coin {
    TENTH_NIS(0.1),
    HALF_NIS(0.5),
    ONE_NIS(1),
    TWO_NIS(2),
    FIVE_NIS(5),
    TEN_NIS(10);
    private final double value;
    Coin(double value) {
        this.value = value;
    }

    public double getValue() {
        return value;
    }

    public static Coin numberToCoin(double value){
        for(Coin coin : Coin.values()){
            if (value == coin.getValue()){
                return coin;
            }
        }
        return null;
    }
}
