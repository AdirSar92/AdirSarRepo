package iotinfrastructure.designPatterns;

/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description: Observer design pattern implementation
 */


public class Publisher<T> {
    private final Dispatcher<T> dispatcher = new Dispatcher<>();

    //default constructor

    public void register(Callback<T> callback) {
        dispatcher.register(callback);
    }

    public void publish(T data) {
        dispatcher.updateAll(data);
    }

    public void close() {
        dispatcher.endService();
    }
}
