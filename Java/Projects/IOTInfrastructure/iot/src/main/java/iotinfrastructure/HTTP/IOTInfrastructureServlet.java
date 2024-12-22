package iotinfrastructure.HTTP;


import iotinfrastructure.parsers.Inflector;
import org.codehaus.jettison.json.JSONException;
import org.codehaus.jettison.json.JSONObject;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.stream.Collectors;

import static iotinfrastructure.testers.IOTServer.IP;
import static iotinfrastructure.testers.IOTServer.PORT;

@WebServlet(urlPatterns = {"/companies/*", "/products/*", "/iots/*"})
public class IOTInfrastructureServlet extends HttpServlet {

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {
        String command = getCommandKey("read", request, true);
        try {
            sendToDB(command, response);
        } catch (IOException | JSONException e) {
            response.getWriter().print("500:Internal error");
        }
    }

    @Override
    protected void doDelete(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        String command = getCommandKey("delete", request, true);
        try {
            sendToDB(command, response);
        } catch (IOException | JSONException e) {
            response.getWriter().print("500:Internal error");
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String command = getCommandKey("create", request, false);
        String requestData = request.getReader().lines().collect(Collectors.joining());
        String DBContent = null;

        try {
            DBContent = JSONParser.jsonToString(command, new JSONObject(requestData));
            sendToDB(DBContent, response);
        } catch (JSONException e) {
            response.getWriter().print("500:Internal error");
        }
    }

    @Override
    protected void doPut(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String command = getCommandKey("update", request, true);
        String requestData = request.getReader().lines().collect(Collectors.joining());
        String DBContent = null;

        try {
            DBContent = request.getServletPath().equals("/iots") ? command + "~" + requestData : JSONParser.jsonToString(command, new JSONObject(requestData));
            sendToDB(DBContent, response);
        } catch (JSONException e) {
            response.getWriter().print("500:Internal error");
        }
    }

    private void sendToDB(String command, HttpServletResponse response) throws IOException, JSONException {
        response.setContentType("application/json");
        ByteBuffer buffer = ByteBuffer.allocate(250);

        try (PrintWriter out = response.getWriter();
             DatagramChannel client = DatagramChannel.open()) {

            client.send(ByteBuffer.wrap(command.getBytes()), new InetSocketAddress(IP, PORT));

            client.receive(buffer);

            JSONObject answer = JSONParser.stringToJson(new String(buffer.array()).trim());
            out.print(answer);
        }
    }

    private String getCommandKey(String type, HttpServletRequest request, boolean includeID) {
        StringBuilder commandKey = new StringBuilder(type);
        commandKey.append(Inflector.getInstance().singularize(request.getServletPath().replace("/", "_")));
        if (includeID && null != request.getPathInfo()) commandKey.append(request.getPathInfo().replace("/", "~"));
        return commandKey.toString();
    }
}

