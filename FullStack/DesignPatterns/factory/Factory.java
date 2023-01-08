/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description:
 */

package il.co.ilrd.designpatterns.factory;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class Factory<K, T, D> {

    private final Map<K, Function<D, T>> map = new HashMap<>();

    public void add(K key, Function<D, T> function) {
        map.put(key, function);
    }

    public T createProduct(K key, D data) {
        return map.get(key).apply(data);
    }
}
