#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using hello::HelloService;
using hello::HelloRequest;
using hello::HelloReply;

class HelloServiceImpl final : public HelloService::Service {
    Status SayHello(ServerContext* context,
                    const HelloRequest* request,
                    HelloReply* reply) override {
        reply->set_message("Hello " + request->name());
        return Status::OK;
    }
};

int main() {
    HelloServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:50051",
                             grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port 50051\n";

    server->Wait();
    return 0;
}
