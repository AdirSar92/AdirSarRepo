package il.co.ilrd.chatserver;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Arrays;
import java.util.Scanner;

public class TCPChatClientSelector {
    private final String ip; //blank final

    public TCPChatClientSelector(String ip) {
        this.ip = ip;
    }

    public void start() {
        Scanner scanner = new Scanner(System.in);
        try (SocketChannel client = SocketChannel.open(new InetSocketAddress(ip, ChatRunner.PORT))) {
            new Thread( () -> {
                try {
                    ByteBuffer buffer = ByteBuffer.allocate(250);

                    while (true) {
                        client.read(buffer);
                        System.out.println(new String(buffer.array()).trim());
                        Arrays.fill(buffer.array(), (byte)0);
                        buffer.clear();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }).start();
            while (true) {
                client.write(ByteBuffer.wrap(scanner.nextLine().getBytes()));
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        TCPChatClientSelector client = new TCPChatClientSelector("localhost");
        client.start();
    }
}
