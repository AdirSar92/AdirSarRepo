package iotinfrastructure.oldCrud;

import iotinfrastructure.CRUD.CRUD;
import iotinfrastructure.parsers.Parser;
import iotinfrastructure.datatables.*;
import org.apache.logging.log4j.LogManager;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class SQLCompanyCRUD implements CRUD<Integer, String> {
    private final PreparedStatement addressStatement; //blank final
    private final PreparedStatement contactStatement;
    private final PreparedStatement creditCardInfoStatement;
    private final PreparedStatement bankInfoStatement;
    private final PreparedStatement companyStatement;
    private final PreparedStatement paymentTypeStatement;
    private final PreparedStatement contactCompanyStatement;
    private final PreparedStatement selectLastIDStatement;

    private final PreparedStatement readStatement;

    private final PreparedStatement deleteCompanyStatement;
    private final PreparedStatement deleteBankInfoStatement;
    private final PreparedStatement deleteCreditCardInfoStatement;
    private final PreparedStatement getQueryPaymentType;

    private final PreparedStatement updateStatement;

    public SQLCompanyCRUD(Connection connection) throws SQLException {
        connection.getMetaData().getColumns(null, null, "Companies", null);
        addressStatement = connection.prepareStatement("INSERT IGNORE INTO Addresses VALUES(?, ?, ?, ?)");
        companyStatement = connection.prepareStatement("INSERT INTO Companies(name, zipcode) VALUES(?,?)");
        contactStatement = connection.prepareStatement("INSERT INTO Contacts (first_name, last_name, email, phone_number) VALUES(?,?,?,?)");
        creditCardInfoStatement = connection.prepareStatement("INSERT INTO Credit_Card_Info(number, expiration_date, cvv) VALUES(?,?,?)");
        bankInfoStatement = connection.prepareStatement("INSERT INTO Bank_Info(bank_number, bank_branch, account_number) VALUES(?,?,?)");
        paymentTypeStatement = connection.prepareStatement("INSERT INTO Payment_Types VALUES(?,?,?)");
        contactCompanyStatement = connection.prepareStatement("INSERT INTO Join_Contacts_Companies VALUES(?,?)");

        readStatement = connection.prepareStatement("SELECT * FROM Companies WHERE ID = ?");

        deleteCompanyStatement = connection.prepareStatement("DELETE FROM Companies WHERE ID = ?");

        deleteBankInfoStatement = connection.prepareStatement("DELETE FROM Bank_Info WHERE ID = ?");
        deleteCreditCardInfoStatement = connection.prepareStatement("DELETE FROM Credit_Card_Info WHERE ID = ?");
        getQueryPaymentType = connection.prepareStatement("SELECT credit_card_ID, bank_ID FROM Payment_Types WHERE company_ID = ?");

        updateStatement = connection.prepareStatement("UPDATE Companies SET name = ? WHERE ID = ?");

        selectLastIDStatement = connection.prepareStatement("SELECT LAST_INSERT_ID()");

        try (Statement useDB = connection.createStatement()) {
            useDB.execute("USE IOT_project_db");
        }
    }

    @Override
    public Integer create(String data) {
        CompanyParser.CompanyData companyData = new CompanyParser().parse(data);
        if (null == companyData) return -1;

        try {
            insertAddress(companyData.getAddress());
            companyData.getCompany().setID(insertCompany(companyData));
            insertContacts(companyData.getContacts());
            insertPaymentInfo(companyData);
            insertContactCompanies(companyData);

        } catch (SQLException e) { //illegal sql argument
            LogManager.getLogger("ErrorLogger").warn("illegal sql argument");
            return -2;
        }
        return companyData.getCompany().getID();
    }

    @Override
    public String read(Integer key) {
        ResultSet resultSet = null;
        try {
            readStatement.setInt(1, key);
            resultSet = readStatement.executeQuery();
            if (resultSet.next()) {
                String name = resultSet.getString("name");
                int zipCode = resultSet.getInt("zipcode");
                return "{Company ID: " + key + ", Name: " + name +
                        ", zipcode: " + zipCode + "}";
            }
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            return null;
        }
        return null;
    }

    @Override
    public void update(Integer key, String data) {
        try {
            updateStatement.setString(1, data);
            updateStatement.setInt(2, key);
            updateStatement.execute();
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            throw new RuntimeException();
        }
    }

    @Override
    public void delete(Integer key) {
        try {
            getQueryPaymentType.setInt(1, key);
            ResultSet resultSet = getQueryPaymentType.executeQuery();

            if (resultSet.next()) {
                deleteCreditCardInfoStatement.setInt(1, resultSet.getInt("credit_card_ID"));
                deleteBankInfoStatement.setInt(1, resultSet.getInt("bank_ID"));

                deleteCreditCardInfoStatement.execute();
                deleteBankInfoStatement.execute();
            }

            deleteCompanyStatement.setInt(1, key);
            deleteCompanyStatement.execute();
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            throw new RuntimeException(e);
        }
    }

    private void insertContactCompanies(CompanyParser.CompanyData companyData) throws SQLException {
        for (Contact currContact : companyData.getContacts()) {
            contactCompanyStatement.setInt(1, companyData.getCompany().getID());
            contactCompanyStatement.setInt(2, currContact.getID());
            contactCompanyStatement.execute();
        }
    }

    private void insertPaymentInfo(CompanyParser.CompanyData companyData) throws SQLException {
        paymentTypeStatement.setInt(1, companyData.getCompany().getID());
        paymentTypeStatement.setNull(2, Types.INTEGER);
        paymentTypeStatement.setNull(3, Types.INTEGER);

        if (companyData.getPaymentType().contains("c")) {
            insertCreditCardInfo(companyData);

            companyData.getCreditCardInfo().setID(getLastInsertedID());
            paymentTypeStatement.setInt(2, companyData.getCreditCardInfo().getID());
        }

        if (companyData.getPaymentType().contains("b")) {
            insertBankInfo(companyData);

            companyData.getBankInfo().setID(getLastInsertedID());
            paymentTypeStatement.setInt(3, companyData.getBankInfo().getID());
        }

        paymentTypeStatement.execute();
    }

    private void insertCreditCardInfo(CompanyParser.CompanyData companyData) throws SQLException {
        creditCardInfoStatement.setString(1, companyData.getCreditCardInfo().getCardNumber());
        creditCardInfoStatement.setDate(2, companyData.getCreditCardInfo().getExpirationDate());
        creditCardInfoStatement.setString(3, companyData.getCreditCardInfo().getCVV());
        creditCardInfoStatement.execute();
    }

    private void insertBankInfo(CompanyParser.CompanyData companyData) throws SQLException {
        bankInfoStatement.setInt(1, companyData.getBankInfo().getBankNumber());
        bankInfoStatement.setInt(2, companyData.getBankInfo().getBankBranch());
        bankInfoStatement.setInt(3, companyData.getBankInfo().getAccountNumber());
        bankInfoStatement.execute();
    }

    private void insertContacts(List<Contact> contacts) throws SQLException {
        for (Contact currContact : contacts) {
            contactStatement.setString(1, currContact.getFirstName());
            contactStatement.setString(2, currContact.getLastName());
            contactStatement.setString(3, currContact.getEmail());
            contactStatement.setString(4, currContact.getPhoneNumber());
            contactStatement.execute();
            currContact.setID(getLastInsertedID());
        }
    }

    private int insertCompany(CompanyParser.CompanyData companyData) throws SQLException {
        companyStatement.setString(1, companyData.getName());
        companyStatement.setInt(2, companyData.getAddress().getZipCode());

        companyStatement.execute();
        return getLastInsertedID();
    }

    private void insertAddress(Address addressData) throws SQLException {
        addressStatement.setInt(1, addressData.getZipCode());
        addressStatement.setString(2, addressData.getStreetName());
        addressStatement.setInt(3, addressData.getStreetNumber());
        addressStatement.setString(4, addressData.getCityName());

        addressStatement.execute();
    }

    private int getLastInsertedID() throws SQLException {
        ResultSet resultSet = selectLastIDStatement.executeQuery();
        return resultSet.next() ? resultSet.getInt(1) : -1;
    }

    public void closeStatements() throws SQLException {
        addressStatement.close();
        contactStatement.close();
        creditCardInfoStatement.close();
        bankInfoStatement.close();
        companyStatement.close();
        paymentTypeStatement.close();
        contactCompanyStatement.close();
        selectLastIDStatement.close();
        readStatement.close();
        deleteCompanyStatement.close();
        deleteBankInfoStatement.close();
        deleteCreditCardInfoStatement.close();
        getQueryPaymentType.close();
        updateStatement.close();
    }

    private static class CompanyParser implements Parser<CompanyParser.CompanyData> {

        @Override
        public CompanyData parse(String data) {
            CompanyData companyData = new CompanyData();
            String[] parsed = data.split("~");
            if (!isRequestValid(parsed)) return null;

            int index = 9;
            companyData.setCompany(new Company(parsed[0]));
            companyData.setAddress(new Address(Integer.parseInt(parsed[1]), parsed[2], Integer.parseInt(parsed[3]), parsed[4]));
            companyData.setPaymentType(parsed[5]);

            switch (parsed[5]) {
                case "c":
                    companyData.setCreditCardInfo(new CreditCardInfo(parsed[6], Date.valueOf(parsed[7]), parsed[8]));
                    break;
                case "b":
                    companyData.setBankInfo(new BankInfo(Integer.parseInt(parsed[6]), Integer.parseInt(parsed[7]), Integer.parseInt(parsed[8])));
                    break;
                case "cb":
                    companyData.setCreditCardInfo(new CreditCardInfo(parsed[6], Date.valueOf(parsed[7]), parsed[8]));
                    companyData.setBankInfo(new BankInfo(Integer.parseInt(parsed[9]), Integer.parseInt(parsed[10]), Integer.parseInt(parsed[11])));
                    index = 12;
                    break;
            }
            for (int i = index; i < parsed.length; i += 4) {
                companyData.addContact(new Contact(parsed[i], parsed[i + 1], parsed[i + 2], parsed[i + 3]));
            }
            return companyData;
        }

        @Override
        public boolean isRequestValid(String[] split) {
            if (13 > split.length) return false;
            int num = 9;
            try {
                Integer.parseInt(split[1]);
                Integer.parseInt(split[3]);
                if (split[5].equals("cb")) {
                    Date.valueOf(split[7]);
                    for (int i = 9; i <= 11; i++) {
                        Integer.parseInt(split[i]);
                    }
                    num = 12;
                } else if (split[5].contains("c")) {
                    Date.valueOf(split[7]);
                } else {
                    for (int i = 6; i <= 8; i++) {
                        Integer.parseInt(split[i]);
                    }
                }
            } catch (IllegalArgumentException e) {
                return false;
            }
            return (0 == (split.length - num) % 4);
        }

        private class CompanyData {
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

            private String getName() {
                return company.getName();
            }

            private String getPaymentType() {
                return paymentType;
            }

            private CreditCardInfo getCreditCardInfo() {
                return creditCardInfo;
            }

            private void setAddress(Address address) {
                this.address = address;
            }

            private void setCompany(Company company) {
                this.company = company;
            }

            private void setPaymentType(String paymentType) {
                this.paymentType = paymentType;
            }

            private void setBankInfo(BankInfo bankInfo) {
                this.bankInfo = bankInfo;
            }

            private void setCreditCardInfo(CreditCardInfo creditCardInfo) {
                this.creditCardInfo = creditCardInfo;
            }

            private void addContact(Contact contact) {
                contacts.add(contact);
            }

            private Address getAddress() {
                return address;
            }

            private Company getCompany() {
                return company;
            }

            private List<Contact> getContacts() {
                return contacts;
            }

            private BankInfo getBankInfo() {
                return bankInfo;
            }
        }
    }
}
