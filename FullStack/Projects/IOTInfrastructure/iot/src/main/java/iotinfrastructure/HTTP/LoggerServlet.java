package iotinfrastructure.HTTP;

import org.codehaus.jettison.json.JSONException;
import org.codehaus.jettison.json.JSONObject;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@WebServlet("/logger/*")
public class LoggerServlet extends HttpServlet {
    private static final List<String> list = new ArrayList<>();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        PrintWriter out = response.getWriter();
        list.forEach(out::println);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {
        response.setContentType("text/html");
        String requestData = request.getReader().lines().collect(Collectors.joining());
        list.add(requestData);
    }
}