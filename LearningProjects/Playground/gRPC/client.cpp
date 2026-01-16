#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using hello::HelloService;
using hello::HelloRequest;
using hello::HelloReply;

int main() {
    auto channel = grpc::CreateChannel(
        "localhost:50051",
        grpc::InsecureChannelCredentials());

    std::unique_ptr<HelloService::Stub> stub =
        HelloService::NewStub(channel);

    HelloRequest request;
    request.set_name("Vjeran");

    HelloReply reply;
    ClientContext context;

    Status status = stub->SayHello(&context, request, &reply);

    if (status.ok()) {
        std::cout << reply.message() << std::endl;
    } else {
        std::cout << "RPC failed\n";
    }

    return 0;
}
