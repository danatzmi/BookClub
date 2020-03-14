//
// Created by dan on 13/01/2020.
//

#include <StompEncDec.h>
#include "CommandsHandler.h"

CommandsHandler::CommandsHandler
(User& user, const vector<string>& input, const string& genre, const string& subscriptionId, int& subId, const string& receiptId, int& recId, string& stomp) :
user(user), input(input), genre(genre), subscriptionId(subscriptionId), subId(subId), receiptId(receiptId), recId(recId), stomp(stomp) {}

void CommandsHandler::handleLogin() {
    stomp.
        append("CONNECT\n").
        append("accept-version:1.2\n").
        append("host:amit.pour\n").
        append("login:" + input[2] + "\n").
        append("passcode:" + input[3] + "\n").
        append("\n").
        append("\n").
        append("\0");
}

void CommandsHandler::handleJoin() {
    stomp.
        append("SUBSCRIBE\n").
        append("destination:" + genre + "\n").
        append("id:" + subscriptionId + "\n").
        append("receipt:" + receiptId + "\n").
        append("\n").
        append("\n").
        append("\0");
    StompMessage stompMessage = StompEncDec::stringToStomp(stomp);
    user.addReceipt(receiptId, stompMessage);
    user.addSubscriptionId(genre, subscriptionId);
    subId = subId + 1;
    recId = recId + 1;
}

void CommandsHandler::handleExit() {
    string id = user.getSubscriptionId(genre);
    stomp.
        append("UNSUBSCRIBE\n").
        append("destination:" + genre + "\n").
        append("id:" + id + "\n").
        append("receipt:" + receiptId + "\n").
        append("\n").
        append("\n").
        append("\0");
    StompMessage stompMessage = StompEncDec::stringToStomp(stomp);
    user.addReceipt(receiptId, stompMessage);
    subId++;
    recId++;
}

void CommandsHandler::handleAdd() {
    string bookName = input[2];
    for (int i = 3; i < (int)input.size(); i++) {
        bookName.append(" ").append(input[i]);
    }
    if(!user.inventoryContains(genre, bookName)) {
        user.addBookToInventory(genre, bookName);
        stomp.
                append("SEND\n").
                append("destination:" + genre + "\n").
                append("\n").
                append(user.getName()).append(" has added the book ").append(bookName + "\n").
                append("\0");
    }
}

void CommandsHandler::handleBorrow() {
    string bookName = input[2];
    for (int i = 3; i < (int)input.size(); i++) {
        bookName.append(" ").append(input[i]);
    }
    user.addBookToWanted(bookName);
    stomp.
        append("SEND\n").
        append("destination:" + genre + "\n").
        append("\n").
        append(user.getName()).append(" wish to borrow ").append(bookName + "\n").
        append("\0");
}

void CommandsHandler::handleReturn() {
    string bookName = input[2];
    for (int i = 3; i < (int)input.size(); i++) {
        bookName.append(" ").append(input[i]);
    }
    string lenderName = user.getLenderName(bookName);
    user.inventoryFindAndDelete(genre, bookName);
    user.removeFromBorrowedBooks(bookName);
    stomp.
        append("SEND\n").
        append("destination:" + genre + "\n").
        append("\n").
        append("Returning ").append(bookName).append(" to ").append(lenderName + "\n").
        append("\0");
}

void CommandsHandler::handleStatus() {
    stomp.
        append("SEND\n").
        append("destination:" + genre + "\n").
        append("\n").
        append("book status\n").
        append("\0");
}

void CommandsHandler::handleLogout() {
    stomp.
        append("DISCONNECT\n").
        append("receipt:" + receiptId + "\n").
        append("username:" + user.getName() + "\n").
        append("\n").
        append("\n").
        append("\0");
    StompMessage stompMessage = StompEncDec::stringToStomp(stomp);
    user.addReceipt(receiptId, stompMessage);
}