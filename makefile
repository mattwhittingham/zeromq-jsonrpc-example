INCLUDES=-I/usr/local/include

all: app app-broker

zmq-server-connector.o: zmq-server-connector.cpp zmq-server-connector.h zmq.hpp zmq-helpers.hpp
	g++ -Wall $(INCLUDES) -c zmq-server-connector.cpp zmq.hpp zmq-helpers.hpp

app-broker: app-broker.cpp
	g++ -Wall $(INCLUDES) -lzmq app-broker.cpp -o app-broker

app: app.cpp zmq-server-connector.o
	g++ -Wall $(INCLUDES) -lzmq -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-server app.cpp -o app zmq-server-connector.o

clean:
	rm *.o
