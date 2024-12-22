package iotinfrastructure.CRUD;

import iotinfrastructure.datatables.Contact;
import iotinfrastructure.datatables.IOTData;
import iotinfrastructure.parsers.Parser;

import java.sql.Connection;
import java.sql.SQLException;

public class IOTManager implements CRUD<Integer, String> {
    private Parser<IOTData> parser = new IOTParser();
    private final CRUD<Integer, String> IOTCRUD;
    private final CRUD<Integer, String> productCRUD;
    private final CRUD<Integer, String> contactsCRUD;
    private final CRUD<Integer, String> IOTcontactsCRUD;

    public IOTManager(Connection connection, String DBName) throws SQLException {
        String splitChar = "~";
        IOTCRUD = new GenericSQLCRUD(connection, "IOT_Reg", DBName,
                false, false, splitChar);
        productCRUD = new GenericSQLCRUD(connection, "Products", DBName,
                false, false, splitChar, "name", "description");
        contactsCRUD = new GenericSQLCRUD(connection, "Contacts", DBName,
                false, false, splitChar, "first_name", "last_name", "email", "phone_number");
        IOTcontactsCRUD = new GenericSQLCRUD(connection, "Join_Contacts_IOT", DBName,
                false, true, splitChar);
    }

    @Override
    public Integer create(String data) {
        IOTData IOTData = parser.parse(data);
        if (null == IOTData) return -1;

        if (null != productCRUD.read(IOTData.getProductID())) {
            int IOTID = IOTCRUD.create(IOTData.getProductID() + "");

            for (Contact contact : IOTData.getContacts()) {
                int contactID = contactsCRUD.create(contact.toString());
                IOTcontactsCRUD.create(IOTID + "~" + contactID);
            }

            return IOTID;
        }
        return -2; //product does not exist
    }

    @Override
    public String read(Integer key) {
        return IOTCRUD.read(key);
    }

    @Override
    public void update(Integer key, String data) {
        throw new IllegalArgumentException();
    }

    @Override
    public void delete(Integer key) {
        IOTCRUD.delete(key);
    }

    @Override
    public void closeResource() {
        IOTCRUD.closeResource();
        IOTcontactsCRUD.closeResource();
        productCRUD.closeResource();
        contactsCRUD.closeResource();
    }

    private class IOTParser implements Parser<IOTData> {
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

    }
}
