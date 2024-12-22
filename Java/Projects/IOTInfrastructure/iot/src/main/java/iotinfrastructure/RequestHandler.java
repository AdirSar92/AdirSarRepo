package iotinfrastructure;

import iotinfrastructure.parsers.ActionParser;
import iotinfrastructure.threadPool.ThreadPool;
import org.apache.logging.log4j.LogManager;

import java.util.logging.LogManager;

public class RequestHandler {
    private final ThreadPool threadPool = new ThreadPool();
    private final ActionParser parser = new ActionParser();

    public void handleRequest(String data, Response response) {
        threadPool.execute(new CommandExecutor(data, response));
    }

    public void shutdownHandlers() {
        threadPool.shutdown();
    }

    private class CommandExecutor implements Runnable {

        private final String data; //blank final
        private final Response response; //blank final

        public CommandExecutor(String data, Response response) {
            this.data = data;
            this.response = response;
        }

        @Override
        public void run() {
            ActionParser.ActionDataRequest dataObject = parser.parse(data);
            if (ActionParser.INVALID_DATA_OBJECT != dataObject &&
                    CommandFactory.getInstance().isServiceAvailable(dataObject.getCommandKey())) {
                Command command = CommandFactory.getInstance().createCommand(dataObject.getCommandKey());
                String answer = command.execute(dataObject.getData());
                response.send(answer);
                LogManager.getLogger("InfoLogger").info(data + " " + answer);
            } else {
                LogManager.getLogger("WarnLogger").warn("404:Invalid operation");
                response.send("404:Invalid operation");
            }
        }
    }
}
