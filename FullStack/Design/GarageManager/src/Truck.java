public class Truck extends Vehicle{
    TypeOfVehicle type = TypeOfVehicle.TRUCK;
    private Status status;
    private final int weightLimit;
    public Truck( int licensePlateID, String description, int yearOfManufacture, int weightLimit) {
        super(licensePlateID, description, yearOfManufacture);
        this.status = super.getStatus();
        this.weightLimit = weightLimit;
    }

    @Override
    public Status getStatus() {
        return status;
    }

    @Override
    public void setStatus(Status status) {
        this.status = status;
    }

    public int getCostToRepair() {
         return this.type.getCostToRepair();
    }

    public int getWeightLimit() {
        return weightLimit;
    }

    public TypeOfVehicle getType() {
        return TypeOfVehicle.TRUCK;
    }
}
