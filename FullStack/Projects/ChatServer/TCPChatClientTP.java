/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 17/09/2022
 * @Description:
 */

package il.co.ilrd.chatserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

import static il.co.ilrd.chatserver.ChatRunner.PORT;

public class TCPChatClientTP {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        try (Socket clientSocket = new Socket("localhost", PORT);
             PrintWriter send = new PrintWriter(clientSocket.getOutputStream(), true);
             BufferedReader get = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()))) {

            send.println("chat.reg.Asaf.hello");

            new Thread(() ->
            {
                try {
                    while (true) {
                        String message = get.readLine();
                        System.out.println(message);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }).start();

            while (true) {
                String input = "chat.msg.Asaf." + scanner.nextLine();
                send.println(input);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
