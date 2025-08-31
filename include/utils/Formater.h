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
    if (str.find("$$$") == std::string::npos) {
      return str;
    }
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
  static void write_beginning(std::ofstream& out) {
    out << "\\documentclass{article}\n";
    out << "\\usepackage[english]{babel}\n";
    out << "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n";
    out << "\\usepackage[T2A]{fontenc}\n";
    out << "\\usepackage[utf8]{inputenc}\n";
    out << "\\usepackage[russian]{babel}\n";
    out << "\\usepackage{amsmath}\n";
    out << "\\usepackage{graphicx}\n";
    out << "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n";
    out << "\\date{}\n";
    out << "\\title{Tasks}\n";
    out << "\\author{Codeforces}\n";
    out << "\\begin{document}\n";
    out << "\\maketitle\n";
  }


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
    
    int cnt = 1;
    for (auto& sample: response.samples()) {
      out << "\\subsection*{Sample " << cnt << "}\n";
      cnt++;
      auto sample_input = format(sample.input_data());
      auto sample_output = format(sample.output_data());

      out << "\\begin{itemize}\n";

      out << "\\item \\textbf{" << "Sample input" << ":} " << '\n';
      out << sample_input << '\n';

      out << "\\item \\textbf{" << "Sample output" << ":} " << '\n';
      out << sample_output << '\n';

      out << "\\end{itemize}\n";
    }
  }


  static void write_ending(std::ofstream& out) {
    out << "\\end{document}\n";
  }
};
