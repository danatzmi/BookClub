package bgu.spl.net.srv;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl implements Connections<StompMessage> {

    private static class ConnectionsImplHolder {
        private static ConnectionsImpl instance = new ConnectionsImpl();
    }
    private Map<Integer, ConnectionHandler<StompMessage>> clientsMap;
    private DataBase dataBase;

    private ConnectionsImpl() {
        clientsMap = new ConcurrentHashMap<>();
        dataBase = DataBase.getInstance();
    }

    public static ConnectionsImpl getInstance() {
        return ConnectionsImplHolder.instance;
    }

    public Map<Integer, ConnectionHandler<StompMessage>> getClientsMap() {
        return clientsMap;
    }

    public boolean send(int connectionId, StompMessage msg) {
        if (clientsMap.get(connectionId) != null) {
            msg.setHeader("Message-id", messageId.toString());
            messageId.incrementAndGet();
            clientsMap.get(connectionId).send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, StompMessage msg) {
        List<Pair<Client, String>> clients = dataBase.getGenresMap().get(channel);
        if(clients != null) {
            for (Pair<Client, String> p : clients) {
                Client c = p.getKey();
                if (c.isLoggedIn()) {
                    StompMessage m = new StompMessage(msg);
                    m.setHeader("subscription", p.getValue());
                    send(c.getConnectionId(), m);
                }
            }
        }

    }

    public void disconnect(int connectionId) {
        try {
            clientsMap.get(connectionId).close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        clientsMap.remove(connectionId);
    }

    public void addConnectedClient(int connectionId, ConnectionHandler<StompMessage> ch) {
        clientsMap.put(connectionId, ch);
    }
}
