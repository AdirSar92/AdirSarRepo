/**
 * @Author: Adir Sarussi
 * @Reviewer: Tzach Halfon
 * @Date: 07.08.22
 * @Description: Singleton different implementations
 */

package il.co.ilrd.singleton;

public class SingletonLazy {
    private static SingletonLazy instance;

    private SingletonLazy() {}

    public static SingletonLazy getInstance() {
        if (null == instance){
            instance = new SingletonLazy();
        }
        return instance;
    }
}
