CC:=g++
PROTOBUF:=/home/oper/.local/bin/protoc-3.21.6.0

.PHONY: clean all

all: navtor-grpc-client

navtor-grpc-client: protocol.proto
	$(PROTOBUF) --grpc_out ./ --cpp_out ./ -I ./ --plugin=protoc-gen-grpc="/home/oper/.local/bin/grpc_cpp_plugin" protocol.proto

#main.o: main.cpp
#	$(CC) -c main.cpp

#clean:
#	rm *.o prog
