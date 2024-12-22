package iotinfrastructure.testers;

import iotinfrastructure.GatewayIOT;

import java.io.IOException;
import java.sql.SQLException;

public class IOTServer {
    public static final String IP = "localhost";
    public static final int PORT = 3838;

    public static void main(String[] args) {
        GatewayIOT gatewayIOT = null;
        try {
            gatewayIOT = new GatewayIOT(IP, PORT, "Asaf", "1234", "/home/asaf/c/jars", "mongodb://localhost:27017");
        } catch (IOException | SQLException e) {
            throw new RuntimeException(e);
        }

        gatewayIOT.start();
    }
}
