#!/bin/bash

python -m grpc_tools.protoc \
    -I./proto \
    --python_out=./server \
    --grpc_python_out=./server \
    ./proto/connection.proto

echo "Python files generated in server/"