/**
 * @Author: Adir Sarussi
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description: Observer design pattern implementation
 */

package il.co.ilrd.designpatterns.observer;

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
