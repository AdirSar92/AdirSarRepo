package iotinfrastructure.CRUD;

import iotinfrastructure.datatables.*;
import iotinfrastructure.parsers.Parser;

import java.sql.Connection;
import java.sql.Date;
import java.sql.SQLException;

import static iotinfrastructure.CRUD.GenericSQLCRUD.badCode;

public class CompanyManager implements CRUD<Integer, String> {
    private final Parser<CompanyData> parser = new CompanyParser();

    private final CRUD<Integer, String> companyCRUD;
    private final CRUD<Integer, String> addressesCRUD;
    private final CRUD<Integer, String> contactsCRUD;
    private final CRUD<Integer, String> creditCardInfoCRUD;
    private final CRUD<Integer, String> bankInfoCRUD;
    private final CRUD<Integer, String> paymentTypeCRUD;
    private final CRUD<Integer, String> joinContactCompanyCRUD;

    public CompanyManager(Connection connection, String DBName) throws SQLException {
        String splitChar = "~";
        addressesCRUD = new GenericSQLCRUD(connection, "Addresses", DBName,
                true, true, splitChar, "street_name", "street_number", "city_name");
        companyCRUD = new GenericSQLCRUD(connection, "Companies", DBName,
                false, false, splitChar, "name");
        contactsCRUD = new GenericSQLCRUD(connection, "Contacts", DBName,
                false, false, splitChar, "first_name", "last_name", "email", "phone_number");
        creditCardInfoCRUD = new GenericSQLCRUD(connection, "Credit_Card_Info", DBName,
                false, false, splitChar, "number", "expiration_date", "cvv");
        bankInfoCRUD = new GenericSQLCRUD(connection, "Bank_Info", DBName,
                false, false, splitChar, "bank_number", "bank_branch", "account_number");
        paymentTypeCRUD = new GenericSQLCRUD(connection, "Payment_Types", DBName,
                false, true, splitChar);
        joinContactCompanyCRUD = new GenericSQLCRUD(connection, "Join_Contacts_Companies", DBName,
                false, true, splitChar);
    }

    @Override
    public Integer create(String data) {
        CompanyData companyData = parser.parse(data);
        if (null == companyData) return -1;

        addressesCRUD.create(companyData.getAddress().toString());
        int companyID = companyCRUD.create(companyData.toString());

        int creditCardID = -1;
        int bankInfoID = -1;

        if (companyData.getPaymentType().contains("c")) {
            creditCardID = creditCardInfoCRUD.create(companyData.getCreditCardInfo().toString());
        }
        if (companyData.getPaymentType().contains("b")) {
            bankInfoID = bankInfoCRUD.create(companyData.getBankInfo().toString());
        }

        StringBuilder paymentData = new StringBuilder(companyID + "~");
        paymentData.append(creditCardID == -1 ? badCode : creditCardID).append("~");
        paymentData.append(bankInfoID == -1 ? badCode : bankInfoID);
        paymentTypeCRUD.create(paymentData.toString());

        for (Contact contact : companyData.getContacts()) {
            int contactID = contactsCRUD.create(contact.toString());
            joinContactCompanyCRUD.create(companyID + "~" + contactID);
        }

        return companyID;
    }

    @Override
    public String read(Integer key) {
        return companyCRUD.read(key);
    }

    @Override
    public void update(Integer key, String data) {
        companyCRUD.update(key, data);
    }

    @Override
    public void delete(Integer key) {
        companyCRUD.delete(key);
    }

    @Override
    public void closeResource() {
        companyCRUD.closeResource();
        contactsCRUD.closeResource();
        creditCardInfoCRUD.closeResource();
        paymentTypeCRUD.closeResource();
        joinContactCompanyCRUD.closeResource();
        bankInfoCRUD.closeResource();
        addressesCRUD.closeResource();
    }

    private class CompanyParser implements Parser<CompanyData> {
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
    }
}
