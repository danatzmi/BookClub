package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class DataBase {

    private static class DataBaseHolder {
        private static DataBase instance = new DataBase();
    }

    private Map<String, Client> registerdClients; // key: user name, value: client
//    private Map<String, Client> loggedInClientsByName; // key: user name, value: client
    private Map<Integer, Client> loggedInClientsById; // key: connection id, value: client
    private Map<String, List<Pair<Client, String>>> genresMap; // key: genre, value: list of <client, subscriptionId>.
    private Map<String, String> subscriptionIdMap; // key: subscriptionId, value: genre


    private DataBase() {
        registerdClients = new HashMap<>();
//        loggedInClientsByName = new HashMap<>();
        loggedInClientsById = new HashMap<>();
        genresMap = new HashMap<>();
        subscriptionIdMap = new HashMap<>();
    }

    public static DataBase getInstance() {
        return DataBaseHolder.instance;
    }

    public Map<String, Client> getRegisterdClients() {
        return registerdClients;
    }

//    public Map<String, Client> getLoggedInClientsByName() {
//        return loggedInClientsByName;
//    }

    public Map<Integer, Client> getLoggedInClientsById() {
        return loggedInClientsById;
    }

    public Map<String, List<Pair<Client, String>>> getGenresMap() {
        return genresMap;
    }

    public Map<String, String> getSubscriptionIdMap() {
        return subscriptionIdMap;
    }

    public List<Pair<Client, String>> getSubscribersList(String genre) {
        return genresMap.get(genre);
    }

}
