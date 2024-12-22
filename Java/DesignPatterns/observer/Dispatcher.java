/**
 * @Author: Adir Sarussi
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description: Observer design pattern implementation
 */

package il.co.ilrd.designpatterns.observer;

import java.util.HashSet;
import java.util.Set;

public class Dispatcher<T>{
    private final Set<Callback<? super T>> callbacks = new HashSet<>();

    //default constructor

    public void register(Callback<T> callback) {
        callback.setDispatcher(this);
        callbacks.add(callback);
    }

    public void unregister(Callback<T> callback) {
        callback.setDispatcher(null);
        callbacks.remove(callback);
    }

    public void updateAll(T data) {
        callbacks.forEach(callback -> callback.update(data));
    }

    public void endService() {
        callbacks.forEach(callback -> callback.stopUpdate());
        callbacks.clear();
    }
}
