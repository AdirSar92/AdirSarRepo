/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 07.08.22
 * @Description: Singleton different implementations
 */

package il.co.ilrd.singleton;

public class SingletonIdiomHolder {

    private SingletonIdiomHolder() {}

    private static class InstanceHolder {
        private static final SingletonIdiomHolder INSTANCE = new SingletonIdiomHolder();
    }

    public static SingletonIdiomHolder getInstance() {
        return InstanceHolder.INSTANCE;
    }
}



