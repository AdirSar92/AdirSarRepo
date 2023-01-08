package iotinfrastructure.designPatterns; /**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description: Observer design pattern implementation
 */


import java.util.function.Consumer;

public class Observer<T> {
    private final Callback<T> callback; // blank final

    public Observer(Consumer<T>consumer, Runnable runnable) {
        this.callback = new Callback<>(consumer, runnable);
    }

    public void register(Publisher<T> publisher) {
        publisher.register(this.callback);
    }

    public void unregister() {
        callback.unregister();
    }

    public T getData() {
        return callback.getData();
    }

}
