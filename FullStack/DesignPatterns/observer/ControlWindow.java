package il.co.ilrd.designpatterns.observer;

import java.util.function.Consumer;

public class ControlWindow<T> extends Observer<T>{
    public ControlWindow(Consumer<T> consumer, Runnable runnable) {
        super(consumer, runnable);
    }
}
