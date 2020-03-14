package bgu.spl.net.srv;

import java.util.LinkedList;
import java.util.List;

public class Client {

    private String userName;
    private String passcode;
    private List<String> genres;
    private boolean loggedIn;
    private int connectionId;

    public Client(String userName, String passcode, int connectionId) {
        this.userName = userName;
        this.passcode = passcode;
        genres = new LinkedList<>();
        loggedIn = true;
        this.connectionId = connectionId;
    }

    public String getUserName() {
        return userName;
    }

    public String getPasscode() {
        return passcode;
    }

    public void addGenre(String genre) {
        genres.add(genre);
    }

    public List<String> getGenres() {
        return genres;
    }

    public boolean isLoggedIn() { return loggedIn; }

    public int getConnectionId() {
        return connectionId;
    }

    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }

    public void setLoggedIn(boolean val) {
        loggedIn = val;
    }

}
