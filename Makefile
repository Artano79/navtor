CC:=g++
CCFLAGS:=-c
CCFLAGS+=-isystem /home/oper/.local/include
CCFLAGS+=-std=gnu++14

gRPC_PATH:=/home/oper/.local

LGFLAGS+=-L$(gRPC_PATH)/lib/ -lgrpc++ -lgrpc++_reflection -lprotobuf -lgrpc -lcares -laddress_sorting
LGFLAGS+=-lre2 -lupb -labsl_raw_hash_set -labsl_hashtablez_sampler -labsl_hash -labsl_city
LGFLAGS+=-labsl_low_level_hash -labsl_statusor -lgpr -lz -labsl_bad_variant_access -labsl_status
LGFLAGS+=-labsl_strerror -labsl_random_distributions -labsl_random_seed_sequences
LGFLAGS+=-labsl_random_internal_pool_urbg -labsl_random_internal_randen -labsl_random_internal_randen_hwaes
LGFLAGS+=-labsl_random_internal_randen_hwaes_impl -labsl_random_internal_randen_slow -labsl_random_internal_platform
LGFLAGS+=-labsl_random_internal_seed_material -labsl_random_seed_gen_exception -labsl_cord -labsl_bad_optional_access
LGFLAGS+=-labsl_cordz_info -labsl_cord_internal -labsl_cordz_functions -labsl_exponential_biased -labsl_cordz_handle
LGFLAGS+=-labsl_str_format_internal -labsl_synchronization -labsl_stacktrace -labsl_symbolize -labsl_debugging_internal
LGFLAGS+=-labsl_demangle_internal -labsl_graphcycles_internal -labsl_malloc_internal -labsl_time -labsl_strings
LGFLAGS+=-labsl_throw_delegate -labsl_int128 -labsl_strings_internal -labsl_base -labsl_spinlock_wait
LGFLAGS+=-labsl_raw_logging_internal -labsl_log_severity -labsl_civil_time -labsl_time_zone
LGFLAGS+=-lssl -lcrypto -lpthread -ldl -lm -lrt


PROTOBUF:=$(gRPC_PATH)/bin/protoc-3.21.6.0

.PHONY: clean all

all: navtor-grpc-client 

navtor-grpc-client: main.o protocol.pb.o protocol.grpc.pb.o
	$(CC) -o navtor-grpc-client main.o protocol.pb.o protocol.grpc.pb.o $(LGFLAGS)  


main.o: main.cpp
	$(CC) $(CCFLAGS) main.cpp

protocol.pb.o: protocol.pb.cc protocol.pb.h
	$(CC) $(CCFLAGS) protocol.pb.cc
	
protocol.grpc.pb.o: protocol.grpc.pb.cc protocol.grpc.pb.h
	$(CC) $(CCFLAGS) protocol.grpc.pb.cc

protofiles: protocol.proto
	$(PROTOBUF) --grpc_out ./ --cpp_out ./ -I ./ --plugin=protoc-gen-grpc="$(gRPC_PROTO)/home/oper/.local/bin/grpc_cpp_plugin" protocol.proto

clean:
	rm *.o *.pb.cc *.pb.h navtor-grpc-client



