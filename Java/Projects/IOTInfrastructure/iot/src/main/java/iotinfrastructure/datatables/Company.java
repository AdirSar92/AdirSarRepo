package iotinfrastructure.datatables;

public class Company {
    private int ID;
    private String name;

    public Company(String name) {
        this.name = name;
    }

    public int getID() {
        return ID;
    }

    public String getName() {
        return name;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

}
