package il.co.ilrd.designpatterns.observer;

import java.util.function.Consumer;

public class DataModel<T> extends Observer<T> {
    public DataModel(Consumer<T> consumer, Runnable runnable) {
        super(consumer, runnable);
    }
}
