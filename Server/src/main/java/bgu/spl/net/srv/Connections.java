package bgu.spl.net.srv;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicInteger;

public interface Connections<T> {

    AtomicInteger messageId = new AtomicInteger(1);

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);
}
