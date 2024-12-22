public class MotorCycle  extends Vehicle{
    TypeOfVehicle type = TypeOfVehicle.MOTORCYCLE;
    private Status status;
    private final boolean isTwoSit;
    public MotorCycle(int licensePlateID, String description, int yearOfManufacture, boolean isTwoSit) {
        super( licensePlateID, description, yearOfManufacture);
        this.isTwoSit = isTwoSit;
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
        return this.type.getCostToRepair();
    }

    public boolean isTwoSit() {
        return isTwoSit;
    }


    public TypeOfVehicle getType() {
        return TypeOfVehicle.MOTORCYCLE;
    }
}
