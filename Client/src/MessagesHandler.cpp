//
// Created by dan on 13/01/2020.
//

#include <MessagesHandler.h>

MessagesHandler::MessagesHandler(ConnectionHandler& ch, User& user, StompMessage& stompMessage, const string& type, const string& body,const string& genre, const vector<string>& splitBody, string& response) :
ch(ch), user(user), stompMessage(stompMessage), type(type), body(body), genre(genre), splitBody(splitBody), response(response) {}

void MessagesHandler::handleBorrow(){
    string bookName = splitBody[4];
    for (int i = 5; i < (int)splitBody.size(); i++) {
        bookName.append(" ").append(splitBody[i]);
    }
    if(user.inventoryContains(genre, bookName)) {
        response.
                append("SEND\n").
                append("destination:" + genre + "\n").
                append("\n").
                append(user.getName() + " has " + bookName + "\n").
                append("\0");
    }
}

void MessagesHandler::handleHas(){
    string otherName = splitBody[0];
    string bookName = splitBody[2];
    for (int i = 3; i < (int)splitBody.size(); i++) {
        bookName.append(" ").append(splitBody[i]);
    }
    if(user.wantedContains(bookName)) {
        user.addBookToInventory(genre, bookName);
        user.addToBorrowedBooks(bookName, otherName);
        user.wantedFindAndDelete(bookName);
        response.
                append("SEND\n").
                append("destination:" + genre + "\n").
                append("\n").
                append("Taking ").append(bookName).append(" from ").append(otherName).append("\n").
                append("\0");
    }
}

void MessagesHandler::handleTaking(){
    string bookName = splitBody[1];
    string otherName = splitBody[(int)splitBody.size() - 1];
    for (int i = 2; i < (int)splitBody.size() && splitBody[i] != "from"; i++) {
        bookName.append(" ").append(splitBody[i]);
    }
    if(user.getName() == otherName) {
        user.inventoryFindAndDelete(genre, bookName);
    }
}

void MessagesHandler::handleStatus() {
    response.
            append("SEND\n").
            append("destination:" + genre + "\n").
            append("\n").
            append(user.getName() + ":" + user.inventoryToString(genre) + "\n").
            append("\0");
}

void MessagesHandler::handleReturning(){
    string bookName = splitBody[1];
    string otherName = splitBody[(int)splitBody.size() - 1];
    for (int i = 2; i < (int)splitBody.size() && splitBody[i] != "to"; i++) {
        bookName.append(" ").append(splitBody[i]);
    }
    if(user.getName() == otherName) {
        user.addBookToInventory(genre, bookName);
    }
}

void MessagesHandler::handleConnected() {
    cout << "Login successful." << endl;
}

void MessagesHandler::handleError() {
    cout << stompMessage.getHeader("message") << endl;
    user.setIsLoggedIn(false);
    ch.close();
}

void MessagesHandler::handleReceipt() {
    string receiptId = stompMessage.getHeader("receipt-id");
    StompMessage message = user.getReceipt(receiptId);
    if(message.getType() == "SUBSCRIBE") {
        string gen = message.getHeader("destination");
        user.registerToGenre(gen);
        cout << "Joined club " << gen << endl;
    }
    if(message.getType() == "UNSUBSCRIBE") {
        string gen = message.getHeader("destination");
        user.unregisterFromGenre(gen);
        cout << "Exited club " << gen << endl;
    }
    if(message.getType() == "DISCONNECT") {
        user.setIsLoggedIn(false);
        ch.close();
    }
}
