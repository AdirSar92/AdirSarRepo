/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 17/09/2022
 * @Description:
 */

package il.co.ilrd.chatserver;

public class ChatRunner {
    public static final int PORT = 6868;

    public static void main(String[] args) {
        ChatServerSelector chatServer = new ChatServerSelector(PORT);
        chatServer.start();
    }
}
