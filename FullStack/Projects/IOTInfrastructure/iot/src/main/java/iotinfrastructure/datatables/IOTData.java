package iotinfrastructure.datatables;

import java.util.ArrayList;
import java.util.List;

public class IOTData {
    private int ID;
    private int productID;
    private final List<Contact> contacts = new ArrayList<>();

    public int getID() {
        return ID;
    }

    public int getProductID() {
        return productID;
    }

    public List<Contact> getContacts() {
        return contacts;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public void setProductID(int productID) {
        this.productID = productID;
    }

    public void setContacts(Contact contact) {
        contacts.add(contact);
    }
}