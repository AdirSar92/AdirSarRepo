package iotinfrastructure.oldCrud;

import iotinfrastructure.CRUD.CRUD;
import iotinfrastructure.GatewayIOT;
import iotinfrastructure.parsers.Parser;
import org.apache.logging.log4j.LogManager;

import java.sql.*;

public class SQLProductCRUD implements CRUD<Integer, String> {

    private final PreparedStatement addressStatement; //blank final
    private final PreparedStatement productReadStatement; //blank final
    private final PreparedStatement selectLastIDStatement; //blank final
    private final PreparedStatement deleteStatement; //blank final
    private final PreparedStatement updateStatement; //blank final

    public SQLProductCRUD(Connection connection) throws SQLException {
        addressStatement = connection.prepareStatement("INSERT INTO Products(name, company_ID, description) VALUES(?, ?, ?)");
        productReadStatement = connection.prepareStatement("SELECT * FROM Products WHERE ID = ?");

        deleteStatement = connection.prepareStatement("DELETE FROM Products WHERE ID = ?");
        updateStatement = connection.prepareStatement("UPDATE Products SET name = ?, description = ? WHERE ID = ?");

        selectLastIDStatement = connection.prepareStatement("SELECT LAST_INSERT_ID()");
        try (Statement useDB = connection.createStatement()) {
            useDB.execute("USE IOT_project_db");
        }
    }

    @Override
    public Integer create(String data) {
        ProductParser.ProductData productData = new ProductParser().parse(data);
        if (null == productData) return -1;

        try {
            insertProduct(productData);
            productData.setID(getInsertedID());
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            return -2;
        }

        return productData.getID();
    }

    @Override
    public String read(Integer key) {
        try {
            productReadStatement.setInt(1, key);
            ResultSet resultSet = productReadStatement.executeQuery();
            if (resultSet.next()) {
                String name = resultSet.getString("name");
                int companyID = resultSet.getInt("company_ID");
                String description = resultSet.getString("description");
                return "{\"Product ID\": \"" + key + "\", \"Name\": \"" + name +
                        "\", \"CompanyID\": \"" + companyID + "\", \"Description\": \"" + description + "\"}";
            }
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            throw new RuntimeException(e);
        }
        return null;
    }

    @Override
    public void update(Integer key, String data) { //update_product~ID~name~description
        String[] split = data.split("~", 2);
        String name = split[0];
        String description = split[1];
        try {
            updateStatement.setString(1, name);
            updateStatement.setString(2, description);
            updateStatement.setInt(3, key);
            updateStatement.execute();
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            throw new RuntimeException(e);
        }
    }

    @Override
    public void delete(Integer key) {
        try {
            deleteStatement.setInt(1, key);
            deleteStatement.execute();
        } catch (SQLException e) {
            LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
            throw new RuntimeException(e);
        }
    }

    private void insertProduct(ProductParser.ProductData companyData) throws SQLException {
        addressStatement.setString(1, companyData.getName());
        addressStatement.setInt(2, companyData.getCompanyID());
        addressStatement.setString(3, companyData.getDescription());
        addressStatement.execute();
    }

    private int getInsertedID() throws SQLException {
        ResultSet resultSet = selectLastIDStatement.executeQuery();
        return resultSet.next() ? resultSet.getInt(1) : -1;
    }

    public void closeStatements() throws SQLException {
        addressStatement.close();
        productReadStatement.close();
        selectLastIDStatement.close();
        deleteStatement.close();
        updateStatement.close();
    }

    private class ProductParser implements Parser<ProductParser.ProductData> {

        @Override
        public ProductData parse(String data) {
            String[] parsed = data.split("~", 3);
            if (!isRequestValid(parsed)) return null;

            return new ProductData(parsed[0], Integer.parseInt(parsed[1]), parsed[2]);
        }

        @Override
        public boolean isRequestValid(String[] split) {
            if (split.length < 3) return false;
            try {
                Integer.parseInt(split[1]);
            } catch (NumberFormatException e) {
                return false;
            }
            return true;
        }

        private class ProductData {
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
    }
}
