cmake_minimum_required(VERSION 3.16)
project(grpc_hello CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Proto file
set(PROTO_FILE hello.proto)

# Generated sources
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(PROTO_SRCS
    ${GENERATED_DIR}/hello.pb.cc
    ${GENERATED_DIR}/hello.grpc.pb.cc)

set(PROTO_HDRS
    ${GENERATED_DIR}/hello.pb.h
    ${GENERATED_DIR}/hello.grpc.pb.h)

# Run protoc
add_custom_command(
    OUTPUT ${PROTO_SRCS} ${PROTO_HDRS}
    COMMAND protobuf::protoc
    ARGS
        --grpc_out ${GENERATED_DIR}
        --cpp_out ${GENERATED_DIR}
        -I ${CMAKE_CURRENT_SOURCE_DIR}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
        ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
)

# Target for generated code
add_library(proto ${PROTO_SRCS})
target_include_directories(proto PUBLIC ${GENERATED_DIR})
target_link_libraries(proto PUBLIC gRPC::grpc++ protobuf::libprotobuf)

# Server
add_executable(server server.cpp)
target_link_libraries(server proto)

# Client
add_executable(client client.cpp)
target_link_libraries(client proto)
