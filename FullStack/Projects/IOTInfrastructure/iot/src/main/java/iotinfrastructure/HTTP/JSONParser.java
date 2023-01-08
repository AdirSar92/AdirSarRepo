package iotinfrastructure.HTTP;

import org.codehaus.jettison.json.JSONArray;
import org.codehaus.jettison.json.JSONException;
import org.codehaus.jettison.json.JSONObject;

import java.util.Iterator;

public class JSONParser {
    @SuppressWarnings("unchecked")
    public static String jsonToString(String commandKey, JSONObject jsonObject) throws JSONException {
        StringBuilder query = new StringBuilder(commandKey);
        Iterator<String> keys = jsonObject.keys();

        while (keys.hasNext()) {
            String key = keys.next();
            if (jsonObject.get(key) instanceof JSONArray) {
                JSONArray jsonArray = jsonObject.getJSONArray(key);

                for (int i = 0; i < jsonArray.length(); i++) {

                    JSONObject innerJSONObject = (JSONObject) jsonArray.get(i);
                    Iterator<String> innerJSONKeys = innerJSONObject.keys();

                    while (innerJSONKeys.hasNext()) {
                        String innerJSONKey = innerJSONKeys.next();
                        query.append("~").append(innerJSONObject.get(innerJSONKey));
                    }
                }
            } else {
                query.append("~").append(jsonObject.get(key));
            }
        }
        return query.toString();
    }

    public static JSONObject stringToJson(String answer) throws JSONException {
        JSONObject jsonObject = new JSONObject();
        String[] split = answer.split(":", 3); //200:OK:ID=3
        jsonObject.put("status code", split[0]);
        jsonObject.put("message", split[1]);

        if (split.length > 2) {
            jsonObject.put("content", new JSONObject(split[2]));
        }

        return jsonObject;
    }
}
