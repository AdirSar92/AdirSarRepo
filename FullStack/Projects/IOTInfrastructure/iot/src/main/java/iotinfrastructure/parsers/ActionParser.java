package iotinfrastructure.parsers;

public class ActionParser implements Parser<ActionParser.ActionDataRequest> {
    public static final ActionDataRequest INVALID_DATA_OBJECT = new ActionDataRequest("X", "X");

    public ActionDataRequest parse(String data) {
        String[] parsedData = data.split("~", 2);
        if (!isRequestValid(parsedData)) return INVALID_DATA_OBJECT;

        return new ActionDataRequest(parsedData[0], parsedData[1]);
    }

    @Override
    public boolean isRequestValid(String[] split) {
        return (split.length > 1);
    }

    public static class ActionDataRequest {
        private final String commandKey; //blank final
        private final String data; //blank final

        public ActionDataRequest(String commandKey, String data) {
            this.commandKey = commandKey;
            this.data = data;
        }

        public String getCommandKey() {
            return commandKey;
        }

        public String getData() {
            return data;
        }
    }

}