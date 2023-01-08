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

import java.io.PrintWriter;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.*;
import java.time.LocalTime;
import java.time.temporal.ChronoUnit;
import java.util.Map;
import java.util.HashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

public class ChatServerTP {
    private final ServiceManager serviceManager = new ServiceManager();
    private final ConnectionManager connectionManager; //blank final

    public ChatServerTP(int PORT) {
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

        private final ExecutorService threadPool = Executors.newCachedThreadPool();
        private final int PORT; //blank final
        private volatile boolean isOn = true;

        public ConnectionManager(int PORT) {
            this.PORT = PORT;
        }

        private void start() {
            threadPool.execute(new TCPManager());
            threadPool.execute(new UDPRunnable());
        }

        private void stop() {
            isOn = false;
            threadPool.shutdown();
        }

        //inner class
        private class TCPManager implements Runnable {

            //default constructor

            @Override
            public void run() {
                try (ServerSocket serverSocket = new ServerSocket(PORT)) {
                    while (isOn) {
                        Socket clientSocket = serverSocket.accept();
                        threadPool.execute(new TCPRunnable(clientSocket));
                    }
                } catch (IOException e) {
                    isOn = false;
                    e.printStackTrace();
                }
            }
        }

        private class TCPRunnable implements Runnable {
            private final Socket clientSocket; // blank final

            public TCPRunnable(Socket clientSocket) {
                this.clientSocket = clientSocket;
            }

            @Override
            public void run() {
                try (BufferedReader reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                     PrintWriter clientOutput = new PrintWriter(clientSocket.getOutputStream(), true)) {

                    String message = reader.readLine();
                    Response response = clientOutput::println;

                    while (null != message) {
                        serviceManager.route(message, response);
                        message = reader.readLine();
                    }

                } catch (IOException e) {
                    isOn = false;
                    e.printStackTrace();
                }
            }
        }

        private class UDPRunnable implements Runnable {
            private final byte[] buffer = new byte[250];
            private final DatagramPacket receivedPacket = new DatagramPacket(buffer, buffer.length);

            //default constructor

            @Override
            public void run() {
                try (DatagramSocket serverSocket = new DatagramSocket(PORT)) {
                    Response response = new UDPResponse(serverSocket);

                    while (isOn) {
                        serverSocket.receive(receivedPacket);
                        String message = new String(receivedPacket.getData(), 0, receivedPacket.getLength());
                        serviceManager.route(message, response);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            private class UDPResponse implements Response {
                private final DatagramSocket serverSocket; //blank final

                public UDPResponse(DatagramSocket serverSocket) {
                    this.serverSocket = serverSocket;
                }

                @Override
                public void send(String message) {
                    int portToSend = receivedPacket.getPort();
                    InetAddress addressToSend = receivedPacket.getAddress();
                    DatagramPacket answer = new DatagramPacket(message.getBytes(), message.length(), addressToSend, portToSend);

                    try {
                        serverSocket.send(answer);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
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
