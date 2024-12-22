package iotinfrastructure.CRUD;

import iotinfrastructure.datatables.ProductData;
import iotinfrastructure.parsers.Parser;

import java.sql.Connection;
import java.sql.SQLException;

public class ProductManager implements CRUD<Integer, String> {
    private final Parser<ProductData> parser = new ProductParser();

    private final CRUD<Integer, String> productCRUD;
    private final CRUD<Integer, String> companyCRUD;

    public ProductManager(Connection connection, String DBName) throws SQLException {
        String splitChar = "~";
        companyCRUD = new GenericSQLCRUD(connection, "Companies", DBName,
                            false, false, splitChar, "name");
        productCRUD = new GenericSQLCRUD(connection, "Products", DBName,
                            false, false, splitChar, "name", "description");
    }

    @Override
    public Integer create(String data) {
        ProductData productData = parser.parse(data);
        if (null == productData) return -1;

        if (null != companyCRUD.read(productData.getCompanyID())) {
            return productCRUD.create(data);
        }
        return -2; //company does not exist
    }

    @Override
    public String read(Integer key) {
        return productCRUD.read(key);
    }

    @Override
    public void update(Integer key, String data) {
        productCRUD.update(key, data);
    }

    @Override
    public void delete(Integer key) {
        productCRUD.delete(key);
    }

    @Override
    public void closeResource() {
        productCRUD.closeResource();
        companyCRUD.closeResource();
    }

    private class ProductParser implements Parser<ProductData> {

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
    }

}
