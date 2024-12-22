package iotinfrastructure.testers;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Arrays;
import java.util.Scanner;

import static iotinfrastructure.testers.IOTServer.IP;
import static iotinfrastructure.testers.IOTServer.PORT;

public class IOTClient {
    public static void main(String[] args) {
        ByteBuffer buffer = ByteBuffer.allocate(250);
        Scanner scanner = new Scanner(System.in);

        try (SocketChannel client = SocketChannel.open(new InetSocketAddress(IP, PORT))) {
            new Thread( () -> {
                try {
                    while (true) {
                        client.read(buffer);
                        System.out.println(new String(buffer.array()).trim());
                        Arrays.fill(buffer.array(), (byte)0);
                        buffer.clear();
                    }
                } catch (IOException ignored) { /* ignored, testing only */ }
            }).start();

            while (true) {
                String userInput = scanner.nextLine();
                client.write(ByteBuffer.wrap(userInput.getBytes()));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
