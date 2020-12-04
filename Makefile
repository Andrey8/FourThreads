all : server.cpp backend.cpp helper.cpp client.cpp
	g++ -o server server.cpp backend.cpp helper.cpp -std=c++11 -lpthread -Wall
	g++ -o client client.cpp -Wall

server : server.cpp backend.cpp helper.cpp
	g++ -o $@ $^ -std=c++11 -lpthread -Wall

client : client.cpp
	g++ -o $@ $^ -Wall

clean : 
	rm -f server client *.o