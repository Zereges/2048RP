CXXFLAGS=-c -std=c++11 -Wall
CXXSERVER=-I2048Server/lib/include
LDFLAGS=-lpthread -lboost_system -lboost_thread -Wl,-R,./2048Server/lib
LDSERVER=-o server -L2048Server/lib -lmysqlcppconn
LDCLIENT=-o client
WITH-DEBUG=-g

# all: server client

all: server

server: ser-main.o session.o player_data.o
	$(CXX) $(LDFLAGS) $(LDSERVER) $+

client: cl-main.o
	$(CXX) $(LDFLAGS) $(LDCLIENT) $+

cl-main.o: 2048Client/src/main.cpp 2048Client/src/client.hpp 2048Client/src/listener.hpp
	$(CXX) $(CXXFLAGS) $(WITH-DEBUG) -o $@ $<

ser-main.o: 2048Server/src/main.cpp  2048Server/src/server.hpp Common/main.hpp
	$(CXX) $(CXXFLAGS) $(CXXSERVER) $(WITH-DEBUG) -o $@ $<

session.o: 2048Server/src/session.cpp 2048Server/src/session.hpp 2048Server/src/session_container.hpp 2048Server/src/base_session.hpp 2048Server/src/player_data.hpp 2048Server/src/sql_connection.hpp
	$(CXX) $(CXXFLAGS) $(CXXSERVER) $(WITH-DEBUG) $<

player_data.o: 2048Server/src/player_data.cpp 2048Server/src/player_data.hpp Common/main.hpp Common/play_event.hpp
	$(CXX) $(CXXFLAGS) $(CXXSERVER) $(WITH-DEBUG) $<

clean:
	$(RM) *.o
