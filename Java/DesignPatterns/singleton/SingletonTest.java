

package il.co.ilrd.singleton;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class SingletonTest {

    @Test
    public void testSingletonEager() {
        SingletonEager singletonEager1 = SingletonEager.getInstance();
        SingletonEager singletonEager2 = SingletonEager.getInstance();
        assertEquals(singletonEager1, singletonEager2);
    }

    @Test
    public void testSingletonLazy() {
        SingletonLazy singletonLazy1 = SingletonLazy.getInstance();
        SingletonLazy singletonLazy2 = SingletonLazy.getInstance();
        assertEquals(singletonLazy1, singletonLazy2);
    }

    @Test
    public void testSingletonLazyDoubleLock() {
        SingletonLazyDoubleLock SingletonLazyDoubleLock1 = SingletonLazyDoubleLock.getInstance();
        SingletonLazyDoubleLock SingletonLazyDoubleLock2 = SingletonLazyDoubleLock.getInstance();
        assertEquals(SingletonLazyDoubleLock1, SingletonLazyDoubleLock2);
    }

    @Test
    public void testSingletonIdiomHolder() {
        SingletonIdiomHolder SingletonIdiomHolder1 = SingletonIdiomHolder.getInstance();
        SingletonIdiomHolder SingletonIdiomHolder2 = SingletonIdiomHolder.getInstance();
        assertEquals(SingletonIdiomHolder1, SingletonIdiomHolder2);
    }

    @Test
    public void testEnum() {
        assertEquals(SingletonEnum.INSTANCE, SingletonEnum.INSTANCE);
    }
}
