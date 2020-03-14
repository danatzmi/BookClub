//
// Created by atzmi@wincs.cs.bgu.ac.il on 12/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPMESSAGE_H
#define BOOST_ECHO_CLIENT_STOMPMESSAGE_H

#include <bits/stdc++.h>
#include <string>
#include <unordered_map>

class StompMessage {

public:
    StompMessage();
    ~StompMessage();
    StompMessage(const StompMessage& other);
    StompMessage* clone();
    void setType(const std::string& type);
    void setHeader(const std::string& k, const std::string& v);
    void setBody(const std::string& body);
    const std::string& getBody() const;
    const std::string& getType() const;
    const std::string& getHeader(const std::string& key);
    const std::unordered_map<std::string, std::string>& getHeadersMap() const;

private:
    std::string type;
    std::string body;
    std::unordered_map<std::string, std::string> headersMap;
};

#endif //BOOST_ECHO_CLIENT_STOMPMESSAGE_H