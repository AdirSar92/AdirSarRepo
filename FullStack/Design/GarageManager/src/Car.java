public class Car extends Vehicle{
    TypeOfVehicle type = TypeOfVehicle.CAR;
    private Status status;
    public Car( int licensePlateID, String description, int yearOfManufacture) {
        super(licensePlateID, description, yearOfManufacture);
        this.status = super.getStatus();
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
        return type.getCostToRepair();
    }



    public TypeOfVehicle getType() {
        return TypeOfVehicle.CAR;
    }
}
