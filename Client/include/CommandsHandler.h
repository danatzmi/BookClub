//
// Created by dan on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_COMMANDSHANDLER_H
#define BOOST_ECHO_CLIENT_COMMANDSHANDLER_H

#include <string>
#include "User.h"

using namespace std;

class CommandsHandler {

public:
    CommandsHandler(User& user, const vector<string>& input,
                    const string& genre, const string& subscriptionId, int& subId,
                    const string& receiptId, int& recId, string& stomp);
    void handleLogin();
    void handleJoin();
    void handleExit();
    void handleAdd();
    void handleBorrow();
    void handleReturn();
    void handleStatus();
    void handleLogout();

private:
    User& user;
    const vector<string>& input;
    const string& genre;
    const string& subscriptionId;
    int& subId;
    const string& receiptId;
    int& recId;
    string& stomp;
};


#endif //BOOST_ECHO_CLIENT_COMMANDSHANDLER_H
