#include <iostream>
#include <grpcpp/grpcpp.h>

#include "protocol.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using CrtServicePilot::CrtService;
using CrtServicePilot::gMIBCAS;
using CrtServicePilot::gMIBCASReply;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(CrtService::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    gMIBCAS request;
    //request.set_name(user);

    // Container for the data we expect from the server.
    gMIBCASReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->generateMapImageByCenterAndScale(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return (reply.result() ? "Result OK" : "Result FAIL");
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

    std::string* generateMapImageByCenterAndScale(
        const double& lat, const double& lon, const double& scale, 
		const uint32_t x, const uint32_t y, 
		const double rot, 
		const std::string& fmt, const std::string& display)
    {
        // Data we are sending to the server.
        gMIBCAS request;
        request.set_requester("user");
        request.set_latcenter(lat);
        request.set_loncenter(lon);
        request.set_scaledenom(scale);
        request.set_rotationdeg(0.0);
        request.set_sizex(x);
        request.set_sizey(y);
        request.set_fmt(fmt);
        request.set_display(display);

        // Container for the data we expect from the server.
        gMIBCASReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->generateMapImageByCenterAndScale(&context, request, &reply);

        // Act upon its status.
        if (status.ok())
        {
            //std::cout << reply.resultdata().size() << "bytes "<< std::endl;
            return reply.release_resultdata();
        }
        else
        {
            //std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            //assure_(!"RPC failed");
        }
        return nullptr;
    }

 private:
  std::unique_ptr<CrtService::Stub> stub_;
};

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  std::string target_str;
  std::string arg_str("--target");
  if (argc > 1) {
    std::string arg_val = argv[1];
    size_t start_pos = arg_val.find(arg_str);
    if (start_pos != std::string::npos) {
      start_pos += arg_str.size();
      if (arg_val[start_pos] == '=') {
        target_str = arg_val.substr(start_pos + 1);
      } else {
        std::cout << "The only correct argument syntax is --target="
                  << std::endl;
        return 0;
      }
    } else {
      std::cout << "The only acceptable argument is --target=" << std::endl;
      return 0;
    }
  } else {
    target_str = "localhost:50051";
  }
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}

