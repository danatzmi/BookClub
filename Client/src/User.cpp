//
// Created by amit on 13/01/2020.
//

#include <boost/algorithm/string/find.hpp>
#include <utility>
#include "User.h"

User::User(string name) : loggedInFlag(true), userName(std::move(name)), inventory{}, borrowedBooks{},
                          receiptsMap{}, subscriptionIdMap{}, wantedBooks{}, registerdGenres{} {}

bool User::wantedFindAndDelete(const string& element){
    auto position = std::find(wantedBooks.begin(), wantedBooks.end(), element);
    if (position != wantedBooks.end()) {
        wantedBooks.erase(position);
        return true;
    }
    return false;
}

bool User::wantedContains(const string& bookName){
    for (const string& book : wantedBooks) {
        if (book == bookName) {
            return true;
        }
    }
    return false;
}

bool User::inventoryFindAndDelete(const string& genre, const string& element) {
    auto position = std::find(inventory[genre].begin(), inventory[genre].end(), element);
    if (position != inventory[genre].end()) {
        inventory[genre].erase(position);
        return true;
    }
    return false;
}

bool User::inventoryContains(const string& genre, const string& bookName) {
    for (const string& book : inventory[genre]) {
        if (book == bookName) {
            return true;
        }
    }
    return false;
}

void User::addBookToInventory(const string& genre, const string& element) {
    inventory[genre].push_back(element);
}

void User::addBookToWanted(const string& element) {
    wantedBooks.push_back(element);
}

void User::addToBorrowedBooks(const string& bookName, const string& borrowerName){
    borrowedBooks[bookName] = borrowerName;
}

void User::removeFromBorrowedBooks(const string& bookName){
    borrowedBooks.erase(bookName);
}

const string& User::getLenderName(const string& bookName) {
    return borrowedBooks[bookName];
}

void User::registerToGenre(const string& genre){
    registerdGenres.push_back(genre);
}

void User::unregisterFromGenre(const string& genre){
    auto position = std::find(registerdGenres.begin(), registerdGenres.end(), genre);
    if (position != registerdGenres.end()) {
        registerdGenres.erase(position);
    }
}

const StompMessage& User::getReceipt(const string& receiptId) {
    return receiptsMap[receiptId];
}

void User::addReceipt(const string& receiptId, const StompMessage& sm) {
    receiptsMap[receiptId] = sm;
}

void User::addSubscriptionId(const string& genre, const string& subscriptionId) {
    subscriptionIdMap[genre] = subscriptionId;
}

string User::inventoryToString(const string& genre) {
    string output;
    for(const string& book : inventory[genre]) {
        output.append(book + ",");
    }
    output = output.substr(0, output.length() - 1);
    return output;
}

const string& User::getName() const {
    return userName;
}

void User::setIsLoggedIn(bool val) {
    loggedInFlag = val;
}

const bool& User::isLoggedIn() const {
    return loggedInFlag;
}

User::~User() {
    inventory.clear();
    borrowedBooks.clear();
    receiptsMap.clear();
    subscriptionIdMap.clear();
    wantedBooks.clear();
    registerdGenres.clear();
}

const string& User::getSubscriptionId(const string &genre) {
    return subscriptionIdMap[genre];
}
