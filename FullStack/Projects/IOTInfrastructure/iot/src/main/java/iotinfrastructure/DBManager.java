package iotinfrastructure;

import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoCollection;
import com.mysql.cj.jdbc.MysqlConnectionPoolDataSource;
import iotinfrastructure.CRUD.*;
import iotinfrastructure.HTTP.JSONParser;
import org.bson.Document;
import org.bson.types.ObjectId;
import org.codehaus.jettison.json.JSONException;
import org.codehaus.jettison.json.JSONObject;

import javax.sql.PooledConnection;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

public class DBManager {
    private final CRUD<Integer, String> companyManager; //blank final
    private final CRUD<Integer, String> productManager; //blank final
    private final CRUD<Integer, String> IOTManager; //blank final
    private final CRUD<ObjectId, JSONObject> IOTUpdateCRUD;

    private final PooledConnection pooledConnection; //blank final
    private final MongoClient mongoClient;


    public DBManager(String userName, String password, String mongoPath) throws SQLException {

        initFactory();

        //create sql connection
        MysqlConnectionPoolDataSource dataSource = new MysqlConnectionPoolDataSource();
        pooledConnection = dataSource.getPooledConnection(userName, password);

        initDatabase(pooledConnection.getConnection());

        //init SQLManagers
        companyManager = new CompanyManager(pooledConnection.getConnection(), "IOT_project_db");
        productManager = new ProductManager(pooledConnection.getConnection(), "IOT_project_db");
        IOTManager = new IOTManager(pooledConnection.getConnection(), "IOT_project_db");

        //init MongoDB CRUD
        mongoClient = MongoClients.create(mongoPath);
        MongoCollection<Document> mongoCollection = mongoClient.getDatabase("iots").getCollection("iot_updates");
        IOTUpdateCRUD = new MongoCRUD(mongoCollection);
    }

    public void close() {
        try {
            pooledConnection.close();
            mongoClient.close();
            companyManager.closeResource();
            productManager.closeResource();
            IOTManager.closeResource();
            IOTUpdateCRUD.closeResource();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private void initDatabase(Connection connection) throws SQLException {
        try (Statement statement = connection.createStatement()) {
            String[] sql = {
                    "CREATE DATABASE IF NOT EXISTS IOT_project_db;",
                    "USE IOT_project_db;",
                    "CREATE TABLE IF NOT EXISTS Addresses (zipcode int NOT NULL PRIMARY KEY, street_name varchar(30) NOT NULL, street_number int NOT NULL, city_name varchar(30) NOT NULL);",
                    "CREATE TABLE IF NOT EXISTS Companies (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, name varchar(30) NOT NULL, zipcode int NOT NULL, CONSTRAINT companies_fk_zipcode FOREIGN KEY (zipcode) REFERENCES Addresses(zipcode) ON DELETE CASCADE);",
                    "CREATE TABLE IF NOT EXISTS Contacts (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, first_name varchar(30) NOT NULL, last_name varchar(30) NOT NULL, email varchar(30) NOT NULL, phone_number varchar(30) NOT NULL);",
                    "CREATE TABLE IF NOT EXISTS Bank_Info (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, bank_number int NOT NULL, bank_branch int NOT NULL, account_number int NOT NULL);",
                    "CREATE TABLE IF NOT EXISTS Credit_Card_Info (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, number varchar(30) NOT NULL, expiration_date DATE NOT NULL, cvv varchar(3) NOT NULL);",
                    "CREATE TABLE IF NOT EXISTS Payment_Types (company_ID int NOT NULL, credit_card_ID int, bank_ID int, CONSTRAINT payment_types_fk_company_id FOREIGN KEY (company_ID) REFERENCES Companies(ID) ON DELETE CASCADE, CONSTRAINT payment_types_fk_bank_id FOREIGN KEY (bank_ID) REFERENCES Bank_Info(ID) ON DELETE CASCADE, CONSTRAINT payment_types_fk_credit_card_id FOREIGN KEY (credit_card_ID) REFERENCES Credit_Card_Info(ID) ON DELETE CASCADE, CONSTRAINT check_payment_type CHECK ((credit_card_ID IS NOT NULL) OR (bank_ID IS NOT NULL)));",
                    "CREATE TABLE IF NOT EXISTS Products (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, name varchar(30) NOT NULL, company_ID int NOT NULL, description varchar(50) NOT NULL, CONSTRAINT products_fk_company_id FOREIGN KEY (company_ID) REFERENCES Companies(ID) ON DELETE CASCADE);",
                    "CREATE TABLE IF NOT EXISTS IOT_Reg (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, product_ID int NOT NULL, CONSTRAINT iot_reg_fk_product_id FOREIGN KEY (product_ID) REFERENCES Products(ID) ON DELETE CASCADE);",
                    "CREATE TABLE IF NOT EXISTS Join_Contacts_Companies (company_ID int NOT NULL, contact_ID int NOT NULL, CONSTRAINT join_contacts_companies_fk_contact_id FOREIGN KEY (contact_ID) REFERENCES Contacts(ID) ON DELETE CASCADE, CONSTRAINT join_contacts_companies_fk_company_id FOREIGN KEY (company_ID) REFERENCES Companies(ID) ON DELETE CASCADE);",
                    "CREATE TABLE IF NOT EXISTS Join_Contacts_IOT (IOT_ID int NOT NULL, contact_ID int NOT NULL, CONSTRAINT join_contacts_IOT_fk_iot_id FOREIGN KEY (IOT_ID) REFERENCES IOT_Reg(ID) ON DELETE CASCADE, CONSTRAINT join_contacts_IOT_fk_contact_id FOREIGN KEY (contact_ID) REFERENCES Contacts(ID) ON DELETE CASCADE);"
            };
            for (String s : sql) {
                statement.execute(s);
            }
        }
    }

    private void initFactory() {
        CommandFactory factory = CommandFactory.getInstance();
        DBCommands commands = new DBCommands();

        /* ======================================== CRUD Company ============================================ */

        factory.add("create_company", commands.createCompany);
        factory.add("read_company", commands.readCompany);
        factory.add("update_company", commands.updateCompany);
        factory.add("delete_company", commands.deleteCompany);

        /* ======================================== CRUD Product ============================================ */

        factory.add("create_product", commands.createProduct);
        factory.add("read_product", commands.readProduct);
        factory.add("delete_product", commands.deleteProduct);
        factory.add("update_product", commands.updateProduct);

        /* ========================================== CRUD IOT ============================================== */

        factory.add("create_iot", commands.createIOT);
        factory.add("read_iot", commands.readIOT);
        factory.add("update_iot", commands.updateIOT);
        factory.add("delete_iot", commands.deleteIOT);

    }

    private class DBCommands {

        private final Command createCompany; //blank final for readability
        private final Command readCompany; //blank final for readability
        private final Command updateCompany; //blank final for readability
        private final Command deleteCompany; //blank final for readability

        private final Command createProduct; //blank final for readability
        private final Command readProduct; //blank final for readability
        private final Command updateProduct; //blank final for readability
        private final Command deleteProduct; //blank final for readability

        private final Command createIOT; //blank final for readability
        private final Command readIOT; //blank final for readability
        private final Command updateIOT; //blank final for readability
        private final Command deleteIOT; //blank final for readability


        private DBCommands() {
            createCompany = (company -> {
                int ID = companyManager.create(company);
                switch (ID) {
                    case -1:
                        return "401:Invalid input";
                    case -2:
                        return "400:Insufficient data";
                    default:
                        return "200:OK:" + "{ID:" + ID + "}";
                }
            });

            readCompany = (record -> {
                if (!isValidID(record)) return "401:Invalid company ID";

                String companyData = companyManager.read(Integer.parseInt(record));
                if (null == companyData) {
                    return "404:No such company";
                }
                return "200:OK:" + companyData;
            });

            updateCompany = (record -> {

                String[] split = record.split("~", 2);
                if (!isValidID(split[0])) return "401:Invalid company ID";

                int ID = Integer.parseInt(split[0]);

                if (null != companyManager.read(ID)) {
                    companyManager.update(ID, split[1]);
                    return "200:OK:" + companyManager.read(ID);
                }
                return "404:No such company ID";

            });

            deleteCompany = (record -> {
                if (!isValidID(record)) return "401:Invalid company ID";

                int ID = Integer.parseInt(record);
                String companyData = companyManager.read(ID);
                companyManager.delete(ID);
                if (null == companyData) {
                    return "404:No such company";
                }
                return "200:OK:" + companyData;
            });

            createProduct = (product -> {
                int ID = productManager.create(product);
                switch (ID) {
                    case -1:
                        return "401:Invalid input";
                    case -2:
                        return "404:No such company ID";
                    default:
                        return "200:OK:" + "{ID:" + ID + "}";
                }
            });

            readProduct = (record -> {
                if (!isValidID(record)) return "401:Invalid product ID";

                String productData = productManager.read(Integer.parseInt(record));
                if (null == productData) {
                    return "404:No such product";
                }
                return "200:OK:" + productData;
            });

            updateProduct = (record -> {
                String[] split = record.split("~", 2);
                if (!isValidID(split[0])) return "401:Invalid product ID";

                int ID = Integer.parseInt(split[0]);

                if (null != productManager.read(ID)) {
                    productManager.update(ID, split[1]);
                    return "200:OK:" + productManager.read(ID);
                }
                return "404:No such product ID";

            });

            deleteProduct = (record -> {

                if (!isValidID(record)) return "401:Invalid company ID";

                int ID = Integer.parseInt(record);
                String product = productManager.read(ID);
                productManager.delete(ID);
                if (null == product) {
                    return "404:No such product ID";
                }
                return "200:Product deleted:" + product;
            });

            createIOT = (iot -> {
                int ID = IOTManager.create(iot);

                switch (ID) {
                    case -1:
                        return "401:Invalid input";
                    case -2:
                        return "404:Product ID Does not exist";
                    default:
                        return "200:OK:" + "{ID:" + ID + "}";
                }
            });

            readIOT = (record -> {
                if (!isValidID(record)) return "401:Invalid IOT ID";

                String IOTQuery = IOTManager.read(Integer.parseInt(record));
                if (null == IOTQuery) {
                    return "404:No such IOT";
                }
                return "200:OK:" + IOTQuery;
            });

            updateIOT = (record -> {
                String[] split = record.split("~", 2);
                if (!isValidID(split[0])) return "401:Invalid IOT ID";

                int ID = Integer.parseInt(split[0]);
                if (null == IOTManager.read(ID)) return "404:No such IOT ID";

                ObjectId objectId = null;
                try {
                    JSONObject jsonObject = new JSONObject(split[1]);
                    jsonObject.put("IOT_ID", ID);
                    objectId = IOTUpdateCRUD.create(jsonObject);
                } catch (JSONException e) {
                    throw new RuntimeException(e);
                }
                return "200:OK:" + "{ID:" + objectId.toString() + "}";
            });

            deleteIOT = (record -> {
                if (!isValidID(record)) return "401:Invalid IOT ID";

                int ID = Integer.parseInt(record);
                String IOT = IOTManager.read(ID);
                IOTManager.delete(ID);

                if (null == IOT) {
                    return "404:No such IOT ID";
                }
                return "200:OK:" + IOT;
            });
        }

        private boolean isValidID(String data) {
            try {
                Integer.parseInt(data);
            } catch (NumberFormatException e) {
                return false;
            }
            return true;
        }
    }
}