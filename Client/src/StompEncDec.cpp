//
// Created by amit on 13/01/2020.
//

#include "StompEncDec.h"

//StompEncDec::StompEncDec() {}

string StompEncDec::stompToString(StompMessage& sm){
    string output;
    output.append(sm.getType() + "\n");
    for(const auto& header : sm.getHeadersMap()) {
        output.append(header.first + ":" + header.second + "\n");
    }
    output.append("\n");
    output.append(sm.getBody());
    output.append("\0");
    return output;
}

StompMessage StompEncDec::stringToStomp(string str) {
    StompMessage output;
    int i = str.find('\n');
    string type = str.substr(0, i);
    output.setType(type);
    str = str.substr(i + 1);
    i = str.find("\n\n");
    string body = str.substr(i + 2);
    body = body.substr(0, body.find('\n'));
    output.setBody(body);
    string headers = str.substr(0, i + 1);
    while((i = headers.find('\n')) >= 0) {
        string header = headers.substr(0, i);
        int colon = header.find(':');
        string key = header.substr(0, colon);
        string value = header.substr(colon + 1);
        output.setHeader(key, value);
        headers = headers.substr(i + 1);
    }
    return output;
}
