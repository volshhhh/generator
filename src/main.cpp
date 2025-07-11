#include <controller/CfController.h>
#include <iostream>

int main() {
  Cf_Controller cf;
  std::vector<std::string> tags = {"math", "graphs", "dp"};
  std::vector<Problem> tt = cf.getProblemsByTags(tags);

  Problem p = tt[3];

  std::cout << p.name << std::endl;
}