public class Vehicle {
    private int VehicleID;
    private SpotSize sizeOfVehicle;

    public Vehicle(int vehicleID, String sizeOfVehicle) {
        VehicleID = vehicleID;
        setSizeOfVehicle(sizeOfVehicle);

    }
    private void setSizeOfVehicle(String sizeOfVehicle){
        switch (sizeOfVehicle){
            case "BIG":
                this.sizeOfVehicle = SpotSize.BIG;
                break;
            case "MED":
                this.sizeOfVehicle = SpotSize.MED;
                break;
            case "SMALL":
                this.sizeOfVehicle = SpotSize.SMALL;
                break;
            default:
                System.out.println("Wrong size");
        }
    }

    public int getVehicleID() {
        return VehicleID;
    }

    public SpotSize getSizeOfVehicle() {
        return sizeOfVehicle;
    }
}
