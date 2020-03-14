
#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "StompMessage.h"

using namespace std;

class User {

public:
    User(string name);
    ~User();
    bool wantedFindAndDelete(const string& element);
    bool wantedContains(const string& bookName);
    bool inventoryFindAndDelete(const string& genre, const string& element);
    bool inventoryContains(const string& genre, const string& bookName);
    void addBookToInventory(const string& genre, const string& element);
    void addBookToWanted(const string& element);
    void addToBorrowedBooks(const string& bookName, const string& userName);
    void removeFromBorrowedBooks(const string& bookName);
    const string& getLenderName(const string& bookName);
    void registerToGenre(const string& genre);
    void unregisterFromGenre(const string& genre);
    const StompMessage& getReceipt(const string& receiptId);
    void addReceipt(const string& receiptId, const StompMessage& sm);
    void addSubscriptionId(const string& genre, const string& subscriptionId);
    string inventoryToString(const string& genre);
    const string& getName() const;
    void setIsLoggedIn(bool val);
    const bool& isLoggedIn() const;
    const string& getSubscriptionId(const string& genre);

private:
    bool loggedInFlag;
    string userName;
    unordered_map<string, vector<string>> inventory; // key: genre, value: list of books.
    unordered_map<string, string> borrowedBooks; // key: book name, value: lender name.
    unordered_map<string, StompMessage> receiptsMap; // key: receipt id, value: message.
    unordered_map<string, string> subscriptionIdMap; // key: topic name, value: subscription id.
    vector<string> wantedBooks; // list of books the user asked to borrow.
    vector<string> registerdGenres; // list of genres that the user is registerd to.
};


#endif //BOOST_ECHO_CLIENT_USER_H
