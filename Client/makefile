CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system
CFLAGS += -pthread
LDFLAGS += -pthread
all:StompBookClubClient
	g++ -o bin/StompBookClubClient.o bin/BookClubClient.o bin/CommandsHandler.o bin/connectionHandler.o bin/MessagesHandler.o bin/StompEncDec.o bin/StompMessage.o bin/User.o $(LDFLAGS)

StompBookClubClient: bin/BookClubClient.o bin/CommandsHandler.o bin/connectionHandler.o bin/MessagesHandler.o bin/StompEncDec.o bin/StompMessage.o bin/User.o

bin/BookClubClient.o: src/BookClubClient.cpp
	g++ $(CFLAGS) -o bin/BookClubClient.o src/BookClubClient.cpp

bin/CommandsHandler.o:  src/CommandsHandler.cpp include/CommandsHandler.h
	g++ $(CFLAGS) -o bin/CommandsHandler.o src/CommandsHandler.cpp

bin/connectionHandler.o: src/connectionHandler.cpp include/connectionHandler.h
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/MessagesHandler.o: src/MessagesHandler.cpp include/MessagesHandler.h
	g++ $(CFLAGS) -o bin/MessagesHandler.o src/MessagesHandler.cpp

bin/StompEncDec.o : src/StompEncDec.cpp include/StompEncDec.h
	g++ $(CFLAGS) -o bin/StompEncDec.o src/StompEncDec.cpp

bin/StompMessage.o:  src/StompMessage.cpp include/StompMessage.h
	g++ $(CFLAGS) -o bin/StompMessage.o src/StompMessage.cpp

bin/User.o:  src/User.cpp include/User.h
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

.PHONY: clean
clean:
	rm -f bin/*

