/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 07.08.22
 * @Description: Singleton different implementations
 */

package il.co.ilrd.singleton;

public class SingletonEager {
    private static final SingletonEager instance = new SingletonEager();

    private SingletonEager() {}

    public static SingletonEager getInstance() {
        return instance;
    }
}
