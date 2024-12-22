package iotinfrastructure.datatables;

public class Contact {
    private int ID;
    private String firstName;
    private String lastName;
    private String email;
    private String phoneNumber;

    public Contact(String firstName, String lastName, String email, String phoneNumber) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phoneNumber = phoneNumber;
    }

    @Override
    public String toString() {
        return firstName + "~" + lastName + "~" + email + "~" + phoneNumber;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public String getEmail() {
        return email;
    }

    public String getPhoneNumber() {
        return phoneNumber;
    }

    public int getID() {
        return ID;
    }
}
