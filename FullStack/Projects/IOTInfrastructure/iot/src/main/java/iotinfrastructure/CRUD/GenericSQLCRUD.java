package iotinfrastructure.CRUD;

import java.sql.*;
import java.sql.Date;
import java.util.*;

public class GenericSQLCRUD implements CRUD<Integer, String> {

    private final Map<String, Integer> colTypesMap = new LinkedHashMap<>();
    public static final String badCode = "BAADC0DE";
    private final String splitChar; //blank final
    private final boolean includePK; //blank final
    private final List<String> columnUpdateNames; //blank final

    private final PreparedStatement selectLastIDStatement; //blank final
    private final PreparedStatement create; //blank final
    private final PreparedStatement read; //blank final
    private final PreparedStatement update; //blank final
    private final PreparedStatement delete; //blank final

    public GenericSQLCRUD(Connection connection, String tableName, String databaseName, boolean insertIgnore,
                          boolean includePK, String splitChar, String... colList) throws SQLException {
        this.splitChar = splitChar;
        this.includePK = includePK;
        this.columnUpdateNames = Arrays.asList(colList);

        try (Statement useDB = connection.createStatement()) {
            useDB.execute("USE " + databaseName);
        }
        initColumnMap(connection, tableName);

        create = buildCreateStatement(connection, tableName, insertIgnore, includePK);
        read = buildBasicStatement(connection, tableName, "SELECT *");
        update = buildUpdateStatement(connection, tableName, colList);
        delete = buildBasicStatement(connection, tableName, "DELETE");
        selectLastIDStatement = connection.prepareStatement("SELECT LAST_INSERT_ID()");
    }

    @Override
    public Integer create(String data) {
        String[] split = data.split(splitChar, colTypesMap.size());
        Iterator<Integer> columnTypeIter = colTypesMap.values().iterator();
        if (!includePK) {
            columnTypeIter.next();
        }
        try {
            for (int i = 0; i < split.length; i++) {
                int type = columnTypeIter.next();
                if (split[i].equals(badCode)) {
                    create.setNull(i + 1, type);
                    continue;
                }
                switch (type) {
                    case Types.INTEGER:
                        create.setInt(i + 1, Integer.parseInt(split[i]));
                        break;
                    case Types.VARCHAR:
                        create.setString(i + 1, split[i]);
                        break;
                    case Types.DATE:
                        create.setDate(i + 1, Date.valueOf(split[i]));
                        break;
                }
            }
            create.execute();
            return getLastInsertedID();

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public String read(Integer key) {
        StringBuilder answer = new StringBuilder("{");
        Iterator<Map.Entry<String, Integer>> columnIterator = colTypesMap.entrySet().iterator();
        try {
            read.setInt(1, key);
            ResultSet resultSet = read.executeQuery();
            if (resultSet.next()) {
                for (int i = 1; i <= colTypesMap.size(); i++) {
                    Map.Entry<String, Integer> currColumn = columnIterator.next();
                    answer.append(currColumn.getKey()).append(":");
                    int type = currColumn.getValue();
                    switch (type) {
                        case Types.INTEGER:
                            answer.append(resultSet.getInt(i));
                            break;
                        case Types.VARCHAR:
                            answer.append(resultSet.getString(i));
                            break;
                        case Types.DATE:
                            answer.append(resultSet.getDate(i));
                            break;
                    }
                    answer.append(", ");
                }
                answer.replace(answer.length() - 2, answer.length() - 1, "}");
                return answer.toString();
            }
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
        return null;
    }

    @Override
    public void update(Integer key, String data) {
        String[] split = data.split(splitChar, columnUpdateNames.size());

        try {
            for (int i = 0; i < split.length; i++) {
                int type = colTypesMap.get(columnUpdateNames.get(i));
                switch (type) {
                    case Types.INTEGER:
                        update.setInt(i + 1, Integer.parseInt(split[i]));
                        break;
                    case Types.VARCHAR:
                        update.setString(i + 1, split[i]);
                        break;
                    case Types.DATE:
                        update.setDate(i + 1, Date.valueOf(split[i]));
                        break;
                }
            }
            update.setInt(columnUpdateNames.size() + 1, key);
            update.execute();
        }  catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void delete(Integer key) {
        try {
            delete.setInt(1, key);
            delete.execute();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void closeResource() {
        try {
            selectLastIDStatement.close();
            create.close();
            read.close();
            update.close();
            delete.close();
        } catch (SQLException e) {
            throw new RuntimeException(e);
        }
    }

    private void initColumnMap(Connection connection, String tableName) throws SQLException {
        PreparedStatement selectFromTable = connection.prepareStatement("SELECT * FROM " + tableName);
        ResultSetMetaData tableMetaData = selectFromTable.executeQuery().getMetaData();

        for (int i = 1; i <= tableMetaData.getColumnCount(); i++) {
            colTypesMap.put(tableMetaData.getColumnName(i), tableMetaData.getColumnType(i));
        }
    }

    private PreparedStatement buildUpdateStatement(Connection connection, String tableName, String[] colList) throws SQLException {
        StringBuilder statement = new StringBuilder("UPDATE ").append(tableName).append(" SET ");

        for (String col : colList) {
            statement.append(col).append(" = ?,");
        }
        statement.deleteCharAt(statement.length() - 1).append(" WHERE ").append(colTypesMap.keySet().iterator().next()).append(" = ?");
        return connection.prepareStatement(statement.toString());
    }

    private PreparedStatement buildBasicStatement(Connection connection, String tableName, String action) throws SQLException {
        return connection.prepareStatement(action + " FROM " + tableName + " WHERE " + colTypesMap.keySet().iterator().next() + " = ?");
    }

    private PreparedStatement buildCreateStatement(Connection connection, String tableName, boolean ignore, boolean includePK) throws SQLException {
        StringBuilder statement = new StringBuilder("INSERT ");

        if (ignore) statement.append("IGNORE ");
        statement.append("INTO ").append(tableName).append(" (");

        Iterator<String> iterator = colTypesMap.keySet().iterator();
        if (!includePK) {
            iterator.next();
        }

        int counter = 0;
        while (iterator.hasNext()) {
            statement.append(iterator.next()).append(",");
            ++counter;
        }

        statement.deleteCharAt(statement.length() - 1).append(") VALUES(");
        while (counter-- > 0) {
            statement.append("?,");
        }
        statement.deleteCharAt(statement.length() - 1).append(")");
        return connection.prepareStatement(statement.toString());
    }

    private int getLastInsertedID() throws SQLException {
        ResultSet resultSet = selectLastIDStatement.executeQuery();

        return resultSet.next() ? resultSet.getInt(1) : -1;
    }
}
