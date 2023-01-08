/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 07.08.22
 * @Description: Singleton different implementations
 */

package il.co.ilrd.singleton;

public class SingletonLazyDoubleLock {
    private static volatile SingletonLazyDoubleLock instance;

    private SingletonLazyDoubleLock() {}

    public static SingletonLazyDoubleLock getInstance() {
        if (null == instance) {
            synchronized (SingletonLazyDoubleLock.class) {
                if (null == instance) {
                    instance = new SingletonLazyDoubleLock();
                }
            }
        }
        return instance;
    }
}
