import java.util.HashMap;
import java.util.Map;

public  class VehicleFactory {
    private static final Map<String,Vehicle> vehicleMap = new HashMap<>();

    public void addVehicle(String type,Vehicle vehicle){
        vehicleMap.put(type,vehicle);
    }

    public Vehicle getVehicle(String type){
        return vehicleMap.get(type);
    }

    public static Map<String,Vehicle> getVehicleMap(){
        return vehicleMap;
    }
}
