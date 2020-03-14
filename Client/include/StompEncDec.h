
#ifndef BOOST_ECHO_CLIENT_STOMPENCDEC_H
#define BOOST_ECHO_CLIENT_STOMPENCDEC_H

#include <boost/algorithm/string.hpp>
#include "../include/StompMessage.h"

using namespace std;

class StompEncDec {

public:
    static string stompToString(StompMessage& sm);
    static StompMessage stringToStomp(string message);
};


#endif //BOOST_ECHO_CLIENT_STOMPENCDEC_H
