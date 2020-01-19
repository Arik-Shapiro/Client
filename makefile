CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: Client
	g++ -pthread -o bin/StompBookClubClient bin/main.o bin/Client.o bin/ClientProtocol.o bin/connectionHandler.o bin/Inventory.o bin/Message.o bin/Transmitter.o $(LDFLAGS) $(DEPS)

Client: bin/main.o bin/Client.o bin/ClientProtocol.o bin/connectionHandler.o bin/Inventory.o bin/Message.o bin/Transmitter.o


bin/main.o: src/main.cpp
	g++ -pthread $(CFLAGS) -o bin/main.o src/main.cpp
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ -pthread $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/ClientProtocol.o: src/ClientProtocol.cpp
	g++ -pthread $(CFLAGS) -o bin/ClientProtocol.o src/ClientProtocol.cpp

bin/Inventory.o: src/Inventory.cpp
	g++ -pthread $(CFLAGS) -o bin/Inventory.o src/Inventory.cpp

bin/Message.o: src/Message.cpp
	g++ -pthread $(CFLAGS) -o bin/Message.o src/Message.cpp

bin/Transmitter.o: src/Transmitter.cpp
	g++ -pthread $(CFLAGS) -o bin/Transmitter.o src/Transmitter.cpp



	
.PHONY: clean
clean:
	rm -f bin/*
