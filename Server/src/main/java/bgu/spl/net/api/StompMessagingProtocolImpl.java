package bgu.spl.net.api;

import bgu.spl.net.srv.*;

import javax.xml.crypto.Data;
import java.util.LinkedList;
import java.util.List;

public class StompMessagingProtocolImpl implements StompMessagingProtocol {

    private boolean shouldTreminate = false;
    private DataBase dataBase;
    private int connectionId;
    private Connections<StompMessage> connections;
    private String clientName;

    public StompMessagingProtocolImpl() {
        this.dataBase = DataBase.getInstance();
        connectionId = 0;
        clientName = "";
    }

    public void start(int connectionId, Connections<StompMessage> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    public void process(StompMessage message) {
        clientName = "";
        StompMessage response = new StompMessage();
        String type = message.getType();
        shouldTreminate = ("DISCONNECT".equals(type));
        switch (type) {
            case "CONNECT": {
                proccessConnect(message, response);
                break;
            }
            case "SUBSCRIBE": {
                proccessSubscribe(message, response);
                break;
            }
            case "UNSUBSCRIBE": {
                proccessUnsubscribe(message, response);
                break;
            }
            case "SEND" : {
                proccessSend(message, response);
                break;
            }
            case "DISCONNECT": {
                proccessDisconnect(message, response);
                break;
            }
        }
        if(!response.getType().equals("")) {
            connections.send(connectionId, response);
            shouldTreminate = ("DISCONNECT".equals(type)) || ("ERROR".equals(response.getType()));
            if (shouldTreminate) {
                close();
            }
        }
    }

    private void proccessConnect(StompMessage message, StompMessage response) {
        String userName = message.getHeader("login");
        String passcode = message.getHeader("passcode");
        if(dataBase.getRegisterdClients().isEmpty() || dataBase.getRegisterdClients().get(userName) == null) {
            Client toRegister = new Client(userName, passcode, connectionId);
            dataBase.getRegisterdClients().put(userName, toRegister);
//            dataBase.getLoggedInClientsByName().put(userName, toRegister);
            dataBase.getLoggedInClientsById().put(connectionId, toRegister);
            response.setType("CONNECTED");
            response.setHeader("version", "1.2");
        } else {
            Client c = dataBase.getLoggedInClientsById().get(connectionId);
            if(c != null) {
                response.setType("ERROR");
                response.setHeader("message","User already logged in");
                clientName = userName;
            } else if(!passcode.equals(dataBase.getRegisterdClients().get(userName).getPasscode())) {
                response.setType("ERROR");
                response.setHeader("message","Wrong password");
                clientName = userName;
            } else {
                Client toRegister = dataBase.getRegisterdClients().get(userName);
                toRegister.setConnectionId(connectionId);
                toRegister.setLoggedIn(true);
                dataBase.getLoggedInClientsById().put(connectionId, toRegister);
                response.setType("CONNECTED");
                response.setHeader("version", "1.2");
            }
        }
    }
    private void proccessSubscribe(StompMessage message, StompMessage response) {
        String genre = message.getHeader("destination");
        String id = message.getHeader("id");
        String receiptId = message.getHeader("receipt");
        Client toAdd = dataBase.getLoggedInClientsById().get(connectionId);
        if (!dataBase.getGenresMap().containsKey(genre)) {
            dataBase.getGenresMap().put(genre, new LinkedList<>());
        }
        dataBase.getGenresMap().get(genre).add(new Pair<>(toAdd, id));
        dataBase.getSubscriptionIdMap().put(id, genre);
        toAdd.addGenre(genre);
        response.setType("RECEIPT");
        response.setHeader("receipt-id", receiptId);
        StompMessage msg = new StompMessage();
        msg.setType("MESSAGE");
        msg.setHeader("destination", genre);
        msg.setBody("User joined club.");
        connections.send(genre, msg);
    }

    private void proccessUnsubscribe(StompMessage message, StompMessage response) {
        String id = message.getHeader("id");
        String genre = dataBase.getSubscriptionIdMap().get(id);
        List<Pair<Client, String>> subscribed = dataBase.getGenresMap().get(genre);
        subscribed.removeIf(pair -> pair.getValue().equals(id));
        StompMessage msg = new StompMessage();
        msg.setType("MESSAGE");
        msg.setHeader("destination", genre);
        msg.setBody("User exited club.");
        connections.send(genre, msg);
    }

    private void proccessSend(StompMessage message, StompMessage response) {
        String messageBody = message.getBody();
        String genre = message.getHeader("destination");
        String[] splittedBody = messageBody.split(" ");
        if(messageBody.contains("added")) {
            String clientName = splittedBody[0];
            StringBuilder bookName = new StringBuilder(splittedBody[5]);
            for(int i = 6; i < splittedBody.length; i++) {
                bookName.append(" ").append(splittedBody[i]);
            }
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody(clientName + " has added the book " + bookName);
            connections.send(genre, msg);
        }
        if(messageBody.contains("borrow")) {
            String clientName = splittedBody[0];
            StringBuilder bookName = new StringBuilder(splittedBody[4]);
            for (int i = 5; i < splittedBody.length; i++) {
                bookName.append(" ").append(splittedBody[i]);
            }
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody(clientName + " wish to borrow " + bookName);
            connections.send(genre, msg);
        }
        if(messageBody.contains("has") && !messageBody.contains("added")) {
            String clientName = splittedBody[0];
            StringBuilder bookName = new StringBuilder(splittedBody[2]);
            for (int i = 3; i < splittedBody.length; i++) {
                bookName.append(" ").append(splittedBody[i]);
            }
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody(clientName + " has " + bookName);
            connections.send(genre, msg);
        }
        if(messageBody.contains("Taking")) {
            String clientName = splittedBody[splittedBody.length - 1];
            StringBuilder bookName = new StringBuilder(splittedBody[1]);
            for (int i = 2; i < splittedBody.length && !splittedBody[i].equals("from"); i++) {
                bookName.append(" ").append(splittedBody[i]);
            }
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody("Taking " + bookName + " from " + clientName);
            connections.send(genre, msg);
        }
        if(messageBody.contains("Returning")) {
            String clientName = splittedBody[splittedBody.length - 1];
            StringBuilder bookName = new StringBuilder(splittedBody[1]);
            for (int i = 2; i < splittedBody.length && !splittedBody[i].equals("to"); i++) {
                bookName.append(" ").append(splittedBody[i]);
            }
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody("Returning " + bookName + " to " + clientName);
            connections.send(genre, msg);
        }
        if(messageBody.contains("status")) {
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody("Book status");
            connections.send(genre, msg);
        }
        if(messageBody.contains(":")) {
            StompMessage msg = new StompMessage();
            msg.setType("MESSAGE");
            msg.setHeader("destination", genre);
            msg.setBody(messageBody);
            connections.send(genre, msg);
        }
    }

    private void proccessDisconnect(StompMessage message, StompMessage response) {
        clientName = message.getHeader("username");
        String receiptId = message.getHeader("receipt");
        response.setType("RECEIPT");
        response.setHeader("receipt-id", receiptId);
    }

    public boolean shouldTerminate() {
        return shouldTreminate;
    }

    public void close() {
        Client toRemove = dataBase.getLoggedInClientsById().get(connectionId);
        List<String> genres = toRemove.getGenres();
        for(String genre : genres) {
            List<Pair<Client, String>> subscribersList = dataBase.getSubscribersList(genre);
            subscribersList.removeIf(p -> p.getKey().getUserName().equals(toRemove.getUserName()));
        }
        dataBase.getLoggedInClientsById().remove(connectionId);
        connections.disconnect(connectionId);
    }

}
