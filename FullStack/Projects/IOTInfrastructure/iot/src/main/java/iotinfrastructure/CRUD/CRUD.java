package iotinfrastructure.CRUD;

import java.io.Serializable;

public interface CRUD<K extends Serializable, D extends Serializable> {
    K create(D data);

    D read(K key);

    void update(K key, D data);

    void delete(K key);
    
    default void closeResource(){
        
    }
}
