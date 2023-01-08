/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description: Observer design pattern implementation
 */

package il.co.ilrd.designpatterns.observer;

import java.util.function.Consumer;

public class Callback<T>{
    private final Consumer<T> updateMethod; // blank final
    private final Runnable stopMethod; // blank final
    private Dispatcher<T> dispatcher = null;
    private T data = null;

    public Callback(Consumer<T> updateMethod, Runnable stopMethod) {
        this.updateMethod = updateMethod;
        this.stopMethod = stopMethod;
    }

    public void update(T data) {
        this.data = data;
        updateMethod.accept(data);
    }

    public void unregister() {
        dispatcher.unregister(this);
    }

    public void stopUpdate() {
        new Thread(stopMethod).start();
    }

    public void setDispatcher(Dispatcher<T> dispatcher) {
        this.dispatcher = dispatcher;
    }

    public T getData() {
        return data;
    }

}
