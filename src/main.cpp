#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include "controller/CfController.h"

#include "proto/connection.grpc.pb.h"
#include "proto/connection.pb.h"

#include <fstream>
#include <iostream>

#include "utils/Formater.h"

std::string getProjectPath() {
#ifdef PROJECT_SOURCE_DIR
  return PROJECT_SOURCE_DIR;
#else
  return std::filesystem::current_path().parent_path().string();
#endif
}

constexpr size_t kProblems = 5;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Nothing to generate...\n";
    return 0;
  }
  // TODO: также передавать кол-во задач, которое хочу нагененрить
  std::vector<std::string> tags;
  tags.reserve(size_t(argc));
  for (size_t i = 1; i < size_t(argc); i++) {
    tags.push_back(argv[i]);
  }

  Cf_Controller controller;
  auto randomProblems = controller.generateRandomProblems(tags, kProblems);

  for (auto &el : randomProblems) {
    std::cout << el << std::endl;
  }
  // return 0;

  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());

  std::unique_ptr<myconnect::Parsing::Stub> stub =
      myconnect::Parsing::NewStub(channel);

  std::ofstream out(getProjectPath() + "/template.tex");

  Formater::write_beginning(out);
  for (std::string &problemLink : randomProblems) {
    myconnect::Request request;
    myconnect::Response response;

    request.set_uri(problemLink);

    grpc::ClientContext context;
    grpc::Status status = stub->ParseUri(&context, request, &response);

    if (status.ok()) {
      std::cout << "Response: OK\n";
      Formater::formAndWriteResponse(response, problemLink, out);
    } else {
      std::cout << "RPC failed: " << status.error_message() << std::endl;
    }
  }
  Formater::write_ending(out);
}
