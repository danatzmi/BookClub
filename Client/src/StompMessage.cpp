//
// Created by atzmi@wincs.cs.bgu.ac.il on 12/01/2020.
//

#include "StompMessage.h"

StompMessage::StompMessage() : type(""), body(""), headersMap() {}

StompMessage::StompMessage(const StompMessage& other) : type(other.type), body(other.body), headersMap() {
    for(const auto& header : other.getHeadersMap()) {
        headersMap[header.first] = header.second;
    }
}

StompMessage* StompMessage::clone() {
    return new StompMessage(*this);
}

void StompMessage::setType(const std::string& type) {
    this->type = type;
}

void StompMessage::setHeader(const std::string& k, const std::string& v) {
    headersMap[k] = v;
}

void StompMessage::setBody(const std::string& body) {
    this->body = body;
}

const std::string& StompMessage::getBody() const {
    return body;
}

const std::string& StompMessage::getType() const {
    return type;
}

const std::string& StompMessage::getHeader(const std::string& key) {
    std::string output = "";
    if(headersMap.find(key) != headersMap.end())
        output = headersMap[key];
    return headersMap[key];
}

const std::unordered_map<std::string, std::string>& StompMessage::getHeadersMap() const {
    return headersMap;
}

StompMessage::~StompMessage() {
    headersMap.clear();
}
