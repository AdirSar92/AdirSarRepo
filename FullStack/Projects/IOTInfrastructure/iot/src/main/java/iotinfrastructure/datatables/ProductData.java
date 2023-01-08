package iotinfrastructure.datatables;

public class ProductData {
    private int ID;
    private final String name;
    private final int companyID;
    private final String description;

    public ProductData(String name, int companyID, String description) {
        this.name = name;
        this.companyID = companyID;
        this.description = description;
    }

    public int getID() {
        return ID;
    }

    public String getName() {
        return name;
    }

    public int getCompanyID() {
        return companyID;
    }

    public String getDescription() {
        return description;
    }

    public void setID(int insertedID) {
        ID = insertedID;
    }
}
