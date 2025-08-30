#include <iostream>

#include "proto/connection.grpc.pb.h"
#include "proto/connection.pb.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

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

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "No links\n";
    return 0;
  }
  std::cout << "I am cpp client\n";

  connect::Request request;
  connect::Response response;

  request.set_uri(argv[1]);

  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<connect::Parsing::Stub> stub =
      connect::Parsing::NewStub(channel);
  grpc::ClientContext context;
  grpc::Status status = stub->ParseUri(&context, request, &response);

  auto title = fix(response.title());
  auto time_limit = fix(response.time_limit());
  auto memory_limit = fix(response.memory_limit());
  auto description = fix(response.description());
  auto input_spec = fix(response.input_spec());
  auto output_spec = fix(response.output_spec());

  std::cout << getProjectPath() << std::endl;
  std::ofstream out(getProjectPath() + "/template.tex", std::ios::app);

  split_time(time_limit);
  split_time(memory_limit);
  out << "\\section{" << "\\href{" << argv[1] << "}{" << title << "}}\n"
      << '\n';
  out << time_limit << '\n' << '\n';
  out << memory_limit << '\n' << '\n';
  out << description << '\n' << '\n';
  out << input_spec << '\n' << '\n';
  out << output_spec << '\n' << '\n';

  out << "\\end{document}\n";
}