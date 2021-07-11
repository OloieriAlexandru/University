INC=/usr/local/ssl/include/
LIB=/usr/local/ssl/lib/

all:
	g++ -std=c++1y -I$(INC) -L$(LIB) -o node_normal.bin node_normal.cpp crypto.cpp command_line.cpp -lcrypto -ldl
	g++ -std=c++1y -I$(INC) -L$(LIB) -o node_manager.bin node_manager.cpp crypto.cpp -lcrypto -ldl

runmanager:
	./node_manager.bin 6000

runnormal:
	./node_normal.bin 6000
