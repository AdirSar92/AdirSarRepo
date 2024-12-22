package iotinfrastructure.CRUD;

import com.mongodb.BasicDBObject;
import com.mongodb.client.MongoCollection;
import org.bson.Document;
import org.bson.types.ObjectId;
import org.codehaus.jettison.json.JSONObject;

public class MongoCRUD implements CRUD<ObjectId, JSONObject> {
    private final MongoCollection<Document> mongoCollection;

    public MongoCRUD(MongoCollection<Document> collection) {
        mongoCollection = collection;
    }

    @Override
    public ObjectId create(JSONObject data) {
        Document document = new Document(data.toMap());
        mongoCollection.insertOne(document);
        return document.getObjectId("_id");
    }

    @Override
    public JSONObject read(ObjectId key) {
        return new JSONObject(mongoCollection.find(new BasicDBObject("_id", key)).first());
    }

    @Override
    public void update(ObjectId key, JSONObject data) {
        mongoCollection.updateOne(new BasicDBObject("_id", key), new BasicDBObject("$set", new Document(data.toMap())));
    }

    @Override
    public void delete(ObjectId key) {
        mongoCollection.deleteOne(new BasicDBObject("_id", key));
    }
}

