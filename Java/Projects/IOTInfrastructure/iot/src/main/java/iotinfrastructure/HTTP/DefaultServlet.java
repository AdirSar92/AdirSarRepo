package iotinfrastructure.HTTP;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;

import static iotinfrastructure.testers.IOTServer.IP;
import static iotinfrastructure.testers.IOTServer.PORT;

@WebServlet("/*")
public class DefaultServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        if (null != req.getPathInfo()) {
            String command = req.getPathInfo().replace("/", "") + "~" + req.getQueryString();
            resp.setContentType("text/html");
            ByteBuffer buffer = ByteBuffer.allocate(250);

            try (PrintWriter out = resp.getWriter();
                 DatagramChannel client = DatagramChannel.open()) {

                client.send(ByteBuffer.wrap(command.getBytes()), new InetSocketAddress(IP, PORT));

                client.receive(buffer);

                String answer = new String(buffer.array()).trim();
                out.print(answer);

            }
        }
    }
        @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        resp.getWriter().print("<h1>404:Page not found</h1>");

    }

    @Override
    protected void doPut(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        resp.getWriter().print("<h1>404:Page not found</h1>");
    }

    @Override
    protected void doDelete(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        resp.getWriter().print("<h1>404:Page not found</h1>");
    }
}
