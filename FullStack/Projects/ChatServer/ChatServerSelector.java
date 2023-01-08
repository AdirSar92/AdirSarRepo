/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 17/09/2022
 * @Description:
 */

package il.co.ilrd.chatserver;

import il.co.ilrd.designpatterns.observer.Observer;
import il.co.ilrd.designpatterns.observer.Publisher;
import il.co.ilrd.networking.Response;

import java.io.IOException;

import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

import java.net.InetSocketAddress;
import java.net.SocketAddress;

import java.time.LocalTime;
import java.time.temporal.ChronoUnit;

import java.util.Arrays;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;

public class ChatServerSelector {
    private final ServiceManager serviceManager = new ServiceManager();
    private final ConnectionManager connectionManager; //blank final

    public ChatServerSelector(int PORT) {
        connectionManager = new ConnectionManager(PORT);
    }

    public void start() {
        connectionManager.start();
    }

    public void stop() {
        connectionManager.stop();
    }

    //inner class

    private class ConnectionManager {

        private final int BUFFER_SIZE = 250;
        private final int PORT; //blank final
        private final String IP = "localhost";
        private volatile boolean isOn = true;

        public ConnectionManager(int PORT) {
            this.PORT = PORT;
        }

        private void start() {

            new Thread(() -> {
                ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);

                try (Selector selector = Selector.open();
                     ServerSocketChannel TCPServerChannel = ServerSocketChannel.open();
                     DatagramChannel UDPChannel = DatagramChannel.open()) {

                    attachServers(selector, TCPServerChannel, UDPChannel);

                    while (isOn) {
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
                    throw new RuntimeException(e);
                }
            }).start();
        }

        private void routeTCPPacket(ByteBuffer buffer, SelectionKey key) throws IOException {
            SocketChannel TCPChannel = (SocketChannel) key.channel();
            Response TCPResponse = (msg -> {
                try {
                    TCPChannel.write(ByteBuffer.wrap(msg.getBytes()));
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            });
            TCPChannel.read(buffer);
            sendMessage(buffer, TCPResponse);
        }

        private void routeUDPPacket(ByteBuffer buffer, DatagramChannel UDPChannel) throws IOException {
            SocketAddress receivedAddress = UDPChannel.receive(buffer);
            Response UDPResponse = msg -> {
                try {
                    UDPChannel.send(ByteBuffer.wrap(msg.getBytes()), receivedAddress);
                } catch (IOException e) {
                    e.printStackTrace();
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
            }
        }

        private void sendMessage(ByteBuffer buffer, Response TCPResponse) {
            if (BUFFER_SIZE != buffer.remaining()) {
                serviceManager.route(new String(buffer.array()).trim(), TCPResponse);
                Arrays.fill(buffer.array(), (byte) 0);
                buffer.clear();
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

        private void stop() {
            isOn = false;
        }
    }

    //inner class
    private static class ServiceManager {

        private final ChatService chatService = new ChatService();

        //default constructor

        private void route(String message, Response response) {
            Message request = Parser.parse(message);
            switch (request.service) {
                case "chat":
                    chatService.activate(request, response);
                    break;
                default:
                    response.send("No service available");
                    break;
            }
        }

        private static class Parser {

            // default constructor

            private static Message parse(String message) {
                StringBuilder userMessage = new StringBuilder();
                String[] parsedMessage = message.split("\\.");
                if (!isValidString(parsedMessage)) return Message.DEFAULT_MESSAGE;
                for (int i = 3; i < parsedMessage.length; ++i) {
                    userMessage.append(parsedMessage[i]).append('.');
                }
                userMessage.deleteCharAt(userMessage.length() - 1);
                return new Message(parsedMessage[0], parsedMessage[1], parsedMessage[2], userMessage.toString());
            }

            private static boolean isValidString(String[] userMessage) {
                return (userMessage.length >= 4);
            }
        }

        private static class Message {
            private static final Message DEFAULT_MESSAGE = new Message("X", "X", "X", "X");
            private final String service; // blank final
            private final String action; // blank final
            private final String ID; // blank final
            private final String message; // blank final

            public Message(String service, String action, String ID, String message) {
                this.service = service;
                this.action = action;
                this.ID = ID;
                this.message = message;
            }
        }

        private static class ChatService {

            private final Publisher<String> publisher = new Publisher<>();
            private final Map<String, Observer<String>> observers = new HashMap<>();

            //default constructor

            private void publish(Message packet, Response response) {
                if (observers.containsKey(packet.ID)) {
                    publisher.publish(formatMessage(packet));
                } else {
                    response.send(packet.ID + " is not registered to this chat service");
                }
            }

            private void register(Message packet, Response response) {
                Observer<String> currentUser = new Observer<>(response::send, null);
                if (null == observers.put(packet.ID, currentUser)) {
                    currentUser.register(publisher);
                    publisher.publish(packet.ID + " joined the server");
                }
            }

            private void unregister(Message packet) {
                Observer<String> user = observers.remove(packet.ID);
                if (null != user) {
                    publisher.publish(packet.ID + " left the server");
                    user.unregister();
                }
            }

            private void activate(Message request, Response response) {
                switch (request.action) {
                    case "reg":
                        register(request, response);
                        break;
                    case "unreg":
                        unregister(request);
                        break;
                    case "msg":
                        publish(request, response);
                        break;
                }
            }

            private String formatMessage(Message packet) {
                return "[" + LocalTime.now().truncatedTo(ChronoUnit.SECONDS) +
                        "] " +
                        packet.ID +
                        ": " +
                        packet.message;
            }
        }
    }
}
