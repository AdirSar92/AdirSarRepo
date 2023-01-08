package iotinfrastructure.oldCrud;

import iotinfrastructure.CRUD.CRUD;
import iotinfrastructure.parsers.Parser;
import iotinfrastructure.datatables.Contact;
import org.apache.logging.log4j.LogManager;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class SQLIOTCRUD implements CRUD<Integer, String> {

    private final PreparedStatement ITRRegStatement; //blank final
    private final PreparedStatement contactStatement; //blank final
    private final PreparedStatement joinIOTContactStatement; //blank final
    private final PreparedStatement IOTUpdateStatement; //blank final
    private final PreparedStatement IOTRReadStatement; //blank final
    private final PreparedStatement selectLastIDStatement; //blank final
    private final PreparedStatement IOTDeleteStatement; //blank final

    public SQLIOTCRUD(Connection connection) throws SQLException {
        ITRRegStatement = connection.prepareStatement("INSERT INTO IOT_Reg(product_ID) VALUES(?)");
        contactStatement = connection.prepareStatement("INSERT INTO Contacts (first_name, last_name, email, phone_number) VALUES(?,?,?,?)");
        joinIOTContactStatement = connection.prepareStatement("INSERT INTO Join_Contacts_IOT VALUES (?, ?)");

        IOTRReadStatement = connection.prepareStatement("SELECT * FROM IOT_Reg WHERE ID = ?");
        IOTUpdateStatement = connection.prepareStatement("UPDATE IOT_Reg SET description = ? WHERE ID = ?");
        IOTDeleteStatement = connection.prepareStatement("DELETE FROM IOT_Reg WHERE ID = ?");

        selectLastIDStatement = connection.prepareStatement("SELECT LAST_INSERT_ID()");
        try (Statement useDB = connection.createStatement()) {
            useDB.execute("USE IOT_project_db");
        }
    }

    @Override
    public Integer create(String data) {
        IOTParser.IOTData IOTData = new IOTParser().parse(data);
        if (null == IOTData) return -1;

        try {
            insertIOT(IOTData);
            insertContacts(IOTData.getContacts());
            insertIOTContacts(IOTData);
        } catch (SQLException e) { //product ID does not exist throws SQLException
            LogManager.getLogger("WarnLogger").warn("product ID does not exist");
            return -2;
        }

        return IOTData.getID();
    }

    @Override
    public String read(Integer key) {
        try {
            IOTRReadStatement.setInt(1, key);
            ResultSet resultSet = IOTRReadStatement.executeQuery();
            if (resultSet.next()) return "{\"IOT ID\": \"" + key + "\", \"product ID\": \"" + resultSet.getString("product_ID") + "\"}";

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return null;
    }

    @Override
    public void update(Integer key, String data) {
        try {
            insertIOTUpdate(key, data);
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void delete(Integer key) {
        try {
            IOTDeleteStatement.setInt(1, key);
            IOTDeleteStatement.execute();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    public void closeStatements() throws SQLException {
        ITRRegStatement.close();
        contactStatement.close();
        joinIOTContactStatement.close();
        IOTUpdateStatement.close();
        IOTRReadStatement.close();
        selectLastIDStatement.close();
        IOTDeleteStatement.close();
    }

    private void insertIOTContacts(IOTParser.IOTData iotData) throws SQLException {
        for (Contact currContact : iotData.getContacts()) {
            joinIOTContactStatement.setInt(1, iotData.getID());
            joinIOTContactStatement.setInt(2, currContact.getID());
            joinIOTContactStatement.execute();
        }
    }

    private void insertIOT(IOTParser.IOTData iotData) throws SQLException {
        ITRRegStatement.setInt(1, iotData.getProductID());
        ITRRegStatement.execute();
        iotData.setID(getInsertedID());
    }

    private void insertContacts(List<Contact> contacts) throws SQLException {
        for (Contact currContact : contacts) {
            contactStatement.setString(1, currContact.getFirstName());
            contactStatement.setString(2, currContact.getLastName());
            contactStatement.setString(3, currContact.getEmail());
            contactStatement.setString(4, currContact.getPhoneNumber());
            contactStatement.execute();

            currContact.setID(getInsertedID());
        }
    }

    private int getInsertedID() throws SQLException {
        ResultSet resultSet = selectLastIDStatement.executeQuery();
        return resultSet.next() ? resultSet.getInt(1) : -1;
    }

    private void insertIOTUpdate(Integer key, String data) throws SQLException {
        IOTUpdateStatement.setInt(1, key);
        IOTUpdateStatement.setString(2, data);
        IOTUpdateStatement.execute();
    }

    private class IOTParser implements Parser<IOTParser.IOTData> {

        @Override
        public IOTData parse(String data) {
            IOTData iotData = new IOTData();
            String[] parsed = data.split("~");
            if (!isRequestValid(parsed)) return null;

            iotData.setProductID(Integer.parseInt(parsed[0]));
            for (int i = 1; i < parsed.length; i += 4) {
                iotData.setContacts(new Contact(parsed[i], parsed[i + 1], parsed[i + 2], parsed[i + 3]));
            }

            return iotData;
        }

        @Override
        public boolean isRequestValid(String[] split) {
            if (5 > split.length || 0 != (split.length - 1) % 4) return false;
            try {
                Integer.parseInt(split[0]);
            } catch (NumberFormatException e) {
                return false;
            }
            return true;
        }

        private class IOTData {
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
    }
}
