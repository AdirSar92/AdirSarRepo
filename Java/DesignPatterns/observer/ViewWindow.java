package il.co.ilrd.designpatterns.observer;

import java.util.function.Consumer;

public class ViewWindow<T> extends Observer<T>{

    public ViewWindow(Consumer<T> consumer, Runnable runnable) {
        super(consumer, runnable);
    }
}
