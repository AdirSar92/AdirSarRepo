package iotinfrastructure;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Set;
import java.util.logging.LogManager;
import java.util.logging.Logger;

public class GatewayIOT {
    private final RequestHandler handler = new RequestHandler();
    private final ConnectionManager connectionManager; // blank final
    private final iotinfrastructure.DBManager DBManager; // blank final
    private final PNPService pnpService; //blank final
    private final Logger logger = LogManager.getLogger(GatewayIOT.class.getSimpleName());

    public GatewayIOT(String ip, int port, String userName, String password, String jarLoadingPath, String mongoPath) throws IOException, SQLException {
        this.connectionManager = new ConnectionManager(ip, port);
        this.DBManager = new DBManager(userName, password, mongoPath);
        this.pnpService = new PNPService(jarLoadingPath);
        pnpService.register(CommandFactory.getInstance().getFactoryAdder());
    }

    public void addCommand(String key, Command command) {
        CommandFactory.getInstance().add(key, command);
    }

    public void start() {
        connectionManager.start();
        pnpService.start();
    }

    public void stop() {
        connectionManager.stop();
        handler.shutdownHandlers();
        DBManager.close();
    }

    private class ConnectionManager {

        private final int BUFFER_SIZE = 250;

        private final int PORT; //blank final
        private final String IP; //blank final
        private final ConnectorThread connectorThread = new ConnectorThread();

        public ConnectionManager(String ip, int PORT) {
            this.IP = ip;
            this.PORT = PORT;
        }

        private void start() {
            connectorThread.start();
        }

        private void stop() {
            connectorThread.isActive = false;
        }

        private class ConnectorThread extends Thread {

            private volatile boolean isActive = true;

            //default constructor

            @Override
            public void run() {
                ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);

                try (Selector selector = Selector.open();
                     ServerSocketChannel TCPServerChannel = ServerSocketChannel.open();
                     DatagramChannel UDPChannel = DatagramChannel.open()) {

                    attachServers(selector, TCPServerChannel, UDPChannel);

                    while (isActive) {
                        selector.select();
                        Set<SelectionKey> selectionKeySet = selector.selectedKeys();

                        for (SelectionKey key : selectionKeySet) {
                            if (key.isAcceptable()) { // TCPManager responsibility
                                handleTCPServer(selector, TCPServerChannel);
                            } else if (key.isReadable()) { //UDPRunnable
                                routeUDPPacket(buffer, UDPChannel);
                            } else if (key.isWritable()) { // TCPRunnable
                                routeTCPPacket(buffer, key);
                            }
                        }
                    }
                } catch (IOException e) {
                    LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
                }
            }

            private void attachServers(Selector selector, ServerSocketChannel channelSocket, DatagramChannel UDPChannel) throws IOException {
                InetSocketAddress serverAddress = new InetSocketAddress(IP, PORT);

                channelSocket.bind(serverAddress);
                channelSocket.configureBlocking(false);
                channelSocket.register(selector, SelectionKey.OP_ACCEPT);

                UDPChannel.bind(serverAddress);
                UDPChannel.configureBlocking(false);
                UDPChannel.register(selector, SelectionKey.OP_READ);
            }

            private void routeTCPPacket(ByteBuffer buffer, SelectionKey key) throws IOException {
                SocketChannel TCPChannel = (SocketChannel) key.channel();
                Response TCPResponse = (msg -> {
                    try {
                        TCPChannel.write(ByteBuffer.wrap(msg.getBytes()));
                    } catch (IOException e) {
                        logger.error(e.getStackTrace());
                    }
                });
                try {
                    TCPChannel.read(buffer);
                    sendMessage(buffer, TCPResponse);
                } catch (IOException e) {
                    LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
                }
            }

            private void routeUDPPacket(ByteBuffer buffer, DatagramChannel UDPChannel) throws IOException {
                SocketAddress receivedAddress = UDPChannel.receive(buffer);
                Response UDPResponse = msg -> {
                    try {
                        UDPChannel.send(ByteBuffer.wrap(msg.getBytes()), receivedAddress);
                    } catch (IOException e) {
                        LogManager.getLogger("ErrorLogger").error(e.getStackTrace());
                    }
                };
                sendMessage(buffer, UDPResponse);
            }

            private void handleTCPServer(Selector selector, ServerSocketChannel TCPServerChannel) throws IOException {
                SocketChannel client = TCPServerChannel.accept();
                if (null != client) {
                    client.configureBlocking(false);
                    client.register(selector, SelectionKey.OP_WRITE);
                    client.write(ByteBuffer.wrap("Connected to server".getBytes()));
                    LogManager.getLogger("InfoLogger").info("Connected to server");
                }
            }

            private void sendMessage(ByteBuffer buffer, Response TCPResponse) {
                if (BUFFER_SIZE != buffer.remaining()) {
                    handler.handleRequest(new String(buffer.array()).trim(), TCPResponse);
                    Arrays.fill(buffer.array(), (byte) 0);
                    buffer.clear();
                }
            }
        }
    }
}
