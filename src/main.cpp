#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include "controller/CfController.h"

#include "proto/connection.grpc.pb.h"
#include "proto/connection.pb.h"

#include <fstream>
#include <iostream>

std::string getProjectPath() {
#ifdef PROJECT_SOURCE_DIR
  return PROJECT_SOURCE_DIR;
#else
  return std::filesystem::current_path().parent_path().string();
#endif
}

void split_time(std::string &limit) {
  const std::string pattern = "test";
  auto it = limit.find(pattern);
  size_t start = it + pattern.size();

  limit += ' ';
  limit += ' ';
  for (size_t i = limit.size() - 1; i >= start; i--) {
    limit[i] = limit[i - 2];
  }

  limit[start] = ':';
  limit[start + 1] = ' ';
}

std::string fix(std::string str) {
  size_t f = 0;

  int cnt_dollar = 0;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] != '$') {
      str[f] = str[i];
      f++;
    } else {
      cnt_dollar++;
      if (cnt_dollar == 1) {
        str[f] = '$';
        f++;
      } else if (cnt_dollar == 6) {
        str[f] = '$';
        f++;
        cnt_dollar = 0;
      }
    }
  }
  str.resize(f);
  return str;
}

void write_response(myconnect::Response &response, std::string &link,
                    std::ofstream &out) {
  auto title = fix(response.title());
  auto time_limit = fix(response.time_limit());
  auto memory_limit = fix(response.memory_limit());
  auto description = fix(response.description());
  auto input_spec = fix(response.input_spec());
  auto output_spec = fix(response.output_spec());

  std::cout << getProjectPath() << std::endl;
  split_time(time_limit);
  split_time(memory_limit);
  out << "\\section{" << "\\href{" << link << "}{" << title << "}}\n" << '\n';
  out << time_limit << '\n' << '\n';
  out << memory_limit << '\n' << '\n';
  out << description << '\n' << '\n';
  out << input_spec << '\n' << '\n';
  out << output_spec << '\n' << '\n';
}

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
  // пока одну задачу (из-за капчи)
  auto randomProblems = controller.generateRandomProblems(tags, 1);

  for (auto &el : randomProblems) {
    std::cout << el << std::endl;
  }
  // return 0;

  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());

  std::unique_ptr<myconnect::Parsing::Stub> stub =
      myconnect::Parsing::NewStub(channel);

  std::ofstream out(getProjectPath() + "/template.tex", std::ios::app);
  for (std::string &problemLink : randomProblems) {
    myconnect::Request request;
    myconnect::Response response;

    request.set_uri(problemLink);

    grpc::ClientContext context;
    grpc::Status status = stub->ParseUri(&context, request, &response);

    if (status.ok()) {
      std::cout << "Response: OK\n";
      write_response(response, problemLink, out);
    } else {
      std::cout << "RPC failed: " << status.error_message() << std::endl;
    }
  }
  out << "\\end{document}\n";
}
