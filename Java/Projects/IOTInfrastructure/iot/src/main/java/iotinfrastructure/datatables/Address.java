package iotinfrastructure.datatables;

public class Address {
    private int zipCode;
    private String streetName;
    private int streetNumber;
    private String cityName;

    public Address(int zipCode, String streetName, int streetNumber, String cityName) {
        this.zipCode = zipCode;
        this.streetName = streetName;
        this.streetNumber = streetNumber;
        this.cityName = cityName;
    }

    @Override
    public String toString() {
        return zipCode + "~" + streetName + "~" + streetNumber + "~" + cityName;
    }

    public int getZipCode() {
        return zipCode;
    }

    public String getStreetName() {
        return streetName;
    }

    public int getStreetNumber() {
        return streetNumber;
    }

    public String getCityName() {
        return cityName;
    }

}