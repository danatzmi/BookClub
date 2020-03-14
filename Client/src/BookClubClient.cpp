//
// Created by amit on 11/01/2020.
//

#include <iostream>
#include <boost/thread.hpp>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <thread>
#include <unordered_map>
#include <User.h>
#include <connectionHandler.h>
#include <StompEncDec.h>
#include <MessagesHandler.h>
#include <CommandsHandler.h>

using namespace std;

class ServerReadTask {

private:
    ConnectionHandler& ch;
    User& user;

public:
    ServerReadTask(ConnectionHandler& ch, User& user) : ch(ch), user(user) {}

    void run() {
        while(user.isLoggedIn()) {
            string line;
            if(ch.getLine(line)) {

                //Decoding incoming message to Stomp frame.
                StompMessage stompMessage(StompEncDec::stringToStomp(line));

                //Splitting message body.
                string body = stompMessage.getBody();
                vector<string> splitBody;
                boost::split(splitBody, body, boost::is_any_of(" "));

                //Init necessary data.
                string type = stompMessage.getType();
                string genre = stompMessage.getHeader("destination");
                string response;

                if(!genre.empty()) {
                    cout << genre << ":" << body << endl;
                }

                MessagesHandler handler(ch, user, stompMessage, type, body, genre, splitBody, response);

                if(!body.empty()) {
                    if (body.find("borrow", 0) != string::npos) { handler.handleBorrow(); }

                    if (body.find("has") != string::npos && body.find("added") == string::npos) {
                        handler.handleHas();
                    }

                    if (body.find("Taking") != string::npos) { handler.handleTaking(); }

                    if (body.find("status") != string::npos) { handler.handleStatus(); }

                    if (body.find("Returning") != string::npos) { handler.handleReturning(); }
                }
                if(!type.empty()) {
                    if (type == "CONNECTED") { handler.handleConnected(); }

                    if (type == "ERROR") { handler.handleError(); }

                    if (type == "RECEIPT") { handler.handleReceipt(); }
                }

                if(!response.empty()) {
                    ch.sendLine(response);
                }
            }
        }
    }
};

class KeyboardReadTask {

private:
    ConnectionHandler& ch;
    User& user;

public:
    KeyboardReadTask(ConnectionHandler& ch, User& user) : ch(ch), user(user) {}

    void run() {
        int subId = 1;
        int recId = 1;
        string userName = user.getName();
        while (user.isLoggedIn()) {

            //Receiving command from keyboard.
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);

            //Splitting command.
            vector<string> input;
            boost::split(input, line, boost::is_any_of(" "));

            //Init necessary data.
            string stomp;
            string command = input[0];
            string genre;
            if (input.size() > 1) {
                genre = input[1];
            }
            string subscriptionId = to_string(subId);
            string receiptId = to_string(recId);

            CommandsHandler handler(user, input, genre, subscriptionId, subId, receiptId, recId, stomp);

            if (command == "login") {
                user.setIsLoggedIn(false);
                handler.handleLogin();
            }

            if (command == "join") { handler.handleJoin(); }

            if (command == "exit") { handler.handleExit(); }

            if (command == "add") { handler.handleAdd(); }

            if (command == "borrow") { handler.handleBorrow(); }

            if (command == "return") { handler.handleReturn(); }

            if (command == "status") { handler.handleStatus(); }

            if (command == "logout") {
                user.setIsLoggedIn(false);
                handler.handleLogout();
            }

            ch.sendLine(stomp);

        }
    }
};

//Asserts legal login command.
void assertLogin(string& line) {
    std::getline(cin, line);
    while(line.find("login") == string::npos) {
        cout << "You must login first!" << endl;
        std::getline(cin, line);
    }
}

int main (int argc, char *argv[]) {
    string line;
    assertLogin(line);
    vector<std::string> input;
    boost::split(input, line, boost::is_any_of(" "));
    string address = input[1];
    string userName = input[2];
    User user(userName);
    int index = address.find(':');
    std::string host = address.substr(0, index);
    short port = atoi(address.substr(index + 1).c_str());
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    string connectFrame;
    connectFrame.
            append("CONNECT\n").
            append("accept-version:1.2\n").
            append("host:amit.pour\n").
            append("login:" + input[2] + "\n").
            append("passcode:" + input[3] + "\n").
            append("\n").
            append("\n").
            append("\0");

    string response;
    if (!connectionHandler.sendLine(connectFrame)) {
        std::cout << "Disconnected. Exiting...\n" << std::endl;
    } else if (!connectionHandler.getLine(response)) {
        std::cout << "Disconnected. Exiting...\n" << std::endl;
    } else {
        StompMessage serverResponse = StompEncDec::stringToStomp(response);
        if (serverResponse.getType() == "CONNECTED") {

            cout << "Login successful." << endl;

            KeyboardReadTask keyboardRead(connectionHandler, user);
            ServerReadTask serverRead(connectionHandler, user);

            thread thread1(&KeyboardReadTask::run, &keyboardRead);
            thread thread2(&ServerReadTask::run, &serverRead);

            thread1.join();
            thread2.join();
        } else if(serverResponse.getType() == "ERROR") {
            cout << serverResponse.getHeader("message") << endl;
        }
    }
}
