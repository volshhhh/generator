#pragma once

#include "proto/connection.pb.h"
#include <fstream>
#include <string>
#include <utility>

class Formater {
private:
  static std::pair<std::string, std::string> split_test(std::string &limit) {
    const std::string pattern = "test";
    auto it = limit.find(pattern);
    size_t size = it + pattern.size();
    return {limit.substr(0, size), limit.substr(size, limit.size() - size)};
  }

  static std::string format(std::string str) {
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

public:
  static void formAndWriteResponse(myconnect::Response &response, std::string &link,
                            std::ofstream &out) {
    auto title = format(response.title());
    auto time_limit = format(response.time_limit());
    auto memory_limit = format(response.memory_limit());
    auto description = format(response.description());
    auto input_spec = format(response.input_spec());
    auto output_spec = format(response.output_spec());

    auto time = split_test(time_limit);
    auto memory = split_test(memory_limit);
    out << "\\section{" << "\\href{" << link << "}{" << title << "}}\n" << '\n';

    out << "\\begin{itemize}\n";

    out << "\\item \\textbf{" << time.first << ":} " << time.second << '\n';
    out << "\\item \\textbf{" << memory.first << ":} " << memory.second << '\n';

    out << "\\end{itemize}\n";

    out << description << '\n' << '\n';

    out << "\\subsection*{Input}\n";
    const std::string input = "Input";
    out << input_spec.substr(input.size()) << '\n' << '\n';

    out << "\\subsection*{Output}\n";
    const std::string output = "Output";
    out << output_spec.substr(output.size()) << '\n' << '\n';
  }
};
