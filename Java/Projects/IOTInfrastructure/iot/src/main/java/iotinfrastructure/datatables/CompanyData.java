package iotinfrastructure.datatables;

import java.util.ArrayList;
import java.util.List;

public class CompanyData {
    private Address address;
    private Company company;
    private String paymentType;
    private BankInfo bankInfo;
    private CreditCardInfo creditCardInfo;
    private final List<Contact> contacts = new ArrayList<>();

    @Override
    public String toString() {
        return company.getName() + "~" + address.getZipCode();
    }

    public String getName() {
        return company.getName();
    }

    public String getPaymentType() {
        return paymentType;
    }

    public CreditCardInfo getCreditCardInfo() {
        return creditCardInfo;
    }

    public void setAddress(Address address) {
        this.address = address;
    }

    public void setCompany(Company company) {
        this.company = company;
    }

    public void setPaymentType(String paymentType) {
        this.paymentType = paymentType;
    }

    public void setBankInfo(BankInfo bankInfo) {
        this.bankInfo = bankInfo;
    }

    public void setCreditCardInfo(CreditCardInfo creditCardInfo) {
        this.creditCardInfo = creditCardInfo;
    }

    public void addContact(Contact contact) {
        contacts.add(contact);
    }

    public Address getAddress() {
        return address;
    }

    public Company getCompany() {
        return company;
    }

    public List<Contact> getContacts() {
        return contacts;
    }

    public BankInfo getBankInfo() {
        return bankInfo;
    }
}
