как установить протобаф на с++

1) установить зависимости

sudo apt install -y cmake ninja-build build-essential autoconf libtool zlib1g-dev libabsl-dev 

2) Сама установка

git clone --recurse-submodules https://github.com/protocolbuffers/protobuf.git
cd protobuf
git checkout v24.4
git submodule update --init --recursive
mkdir build && cd build
cmake ..     -DCMAKE_BUILD_TYPE=Release     -Dprotobuf_BUILD_TESTS=OFF
make -j$(nproc)
sudo make install
sudo ldconfig

3) проверка
protoc --version

вывод: libprotoc 24.4


как установить grpc

git clone --recurse-submodules -b v1.59.0 https://github.com/grpc/grpc
cd grpc
mkdir -p cmake/build
cd cmake/build

# Сборка и установка
cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF ../..
make -j$(nproc)
sudo make install
sudo ldconfig