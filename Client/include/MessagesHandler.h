//
// Created by dan on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_MESSAGESHANDLER_H
#define BOOST_ECHO_CLIENT_MESSAGESHANDLER_H


#include <string>
#include <vector>
#include "User.h"
#include "connectionHandler.h"

using namespace std;

class MessagesHandler {
public:
    MessagesHandler(ConnectionHandler& ch, User& user, StompMessage& stompMessage, const string& type, const string& body,
                    const string& genre, const vector<string>& splitBody, string& response);
    void handleBorrow();
    void handleHas();
    void handleTaking();
    void handleStatus();
    void handleReturning();
    void handleConnected();
    void handleError();
    void handleReceipt();

private:
    ConnectionHandler& ch;
    User& user;
    StompMessage& stompMessage;
    const string& type;
    const string& body;
    const string& genre;
    const vector<string>& splitBody;
    string& response;
};


#endif //BOOST_ECHO_CLIENT_MESSAGESHANDLER_H
