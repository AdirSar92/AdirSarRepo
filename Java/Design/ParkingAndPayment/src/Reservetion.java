import java.time.Duration;
import java.time.LocalTime;

public class Reservetion{
        private static int reserveIDGlobal = 1;
        private final int reserevID;
        private final LocalTime startTime;
        private  LocalTime endTime;
        private final Vehicle vehicle;
        private final int spotID;
        private final SpotSize spotSize;


         Reservetion(Vehicle vehicle, int spotID, SpotSize spotSize) {
             this.spotID = spotID;
             this.spotSize = spotSize;
             this.reserevID = getReserveID();
            this.startTime = LocalTime.now();
            this.vehicle = vehicle;
        }

    public SpotSize getSpotSize() {
        return spotSize;
    }

    public int getReserevID() {
        return reserevID;
    }

    public LocalTime getStartTime() {
        return startTime;
    }

    public LocalTime getEndTime() {
        return endTime;
    }

    public int getSpotID() {
        return spotID;
    }

    private int getReserveID(){
        int res = reserveIDGlobal;
        reserveIDGlobal++;
        return res;
    }

    public void endParking(){
             this.endTime = LocalTime.now();
    }

    public Duration getOverallTime(){
             return Duration.between(endTime,startTime);
    }

    public int getVehicleID(){
             return  vehicle.getVehicleID();
    }

}
