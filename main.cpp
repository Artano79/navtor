#include <iostream>
#include <fstream>
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

    const std::string generateMapImageByCenterAndScale(
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
			std::cout << "Chart recieved: lat= " << lat << " lon=" << lon << " scale=" << scale;
			std::cout << " x=" << x << " y=" << y << " rot" << rot; 
			std::cout << " fmt = " << fmt << " display=" <<  display << std::endl;
            
			std::cout << "Status=" << reply.result() << " Size=" << reply.resultdata().size() << std::endl;
			return reply.resultdata();
        }
        else
        {
			//static std::string global_string("fghaksdjfghikaiklg");
			//return global_string;
			throw std::runtime_error("Chart server remote call FAIL"); 
		}
    }

 private:
  std::unique_ptr<CrtService::Stub> stub_;
};

int main(int argc, char** argv) {
	try{
		std::string target_str;
		std::string arg_str("--target");
		if (argc > 1) {
			std::string arg_val = argv[1];
			size_t start_pos = arg_val.find(arg_str);
			if (start_pos != std::string::npos) {
				start_pos += arg_str.size();
				if (arg_val[start_pos] == '=') {
					target_str = arg_val.substr(start_pos + 1);
				} 
				else {
			        throw std::runtime_error("The only correct argument syntax is --target=");
				}
			} 
			else {
				throw std::runtime_error("The only acceptable argument is --target=");
			}
		} 
		else {
		    target_str = "localhost:50051";
		}
		GreeterClient greeter(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
		const std::string reply = greeter.generateMapImageByCenterAndScale(60.0,30.0,1.e6,640,480,
									0.0,"image/jpeg","Standard");
		//if (!std::filesystem::exists(outDir.c_str()))
		//	std::filesystem::create_directories(outDir.c_str());


		std::string name = "./out.jpeg";

		std::ofstream fout(name, std::ios::binary);

		fout.write(reply.c_str(), reply.size());
	
	}
	catch(const std::exception& e){
		std::cout << "Exception: " << e.what() << std::endl;
	}
  return 0;
}

