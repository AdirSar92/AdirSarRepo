
public abstract class Vehicle {
    private final int licensePlateID;
    private Status status;
    private final String description;
    private final int yearOfManufacture;

    public Vehicle(int licensePlateID, String description, int yearOfManufacture) {
        this.licensePlateID = licensePlateID;
        this.description = description;
        this.yearOfManufacture = yearOfManufacture;
        this.status = Status.NEW;
    }

    public Status getStatus() {
        return status;
    }

    public void setStatus(Status status) {
        this.status = status;
    }
    public int getCost(){
        return this.getType().getCostToRepair();
    }

    public abstract TypeOfVehicle getType();

    public int getLicensePlateID() {
        return licensePlateID;
    }

    public String getDescription() {
        return description;
    }

    public int getYearOfManufacture() {
        return yearOfManufacture;
    }
}


