#include "CfController.h"
#include "entities/Problem.h"
#include "entities/ProblemStatistics.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <random>

Cf_Controller::Cf_Controller() {
  curl = curl_easy_init();
  auto &factory = JsonConverterFactory::getInstance();
  factory.initConverters();
}
Cf_Controller::~Cf_Controller() { curl_easy_cleanup(curl); }

std::vector<Problem>
Cf_Controller::getProblemsByTags(std::vector<std::string> &tags) {
  std::string raw_response;
  std::string address = api_address + "problemset.problems?tags=";

  for (auto &tag : tags) {
    address += tag + ";";
  }
  std::cout << address << std::endl;

  curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &raw_response);

  json parsed_response;

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
    return {};
  }

  if (raw_response.empty()) {
    std::cerr << "Empty response from server" << std::endl;
    return {};
  }

  try {
    parsed_response = json::parse(raw_response);
  } catch (json::parse_error &e) {
    std::cerr << "JSON Error: " << e.what() << std::endl;
  }

  auto &factory = JsonConverterFactory::getInstance();
  std::vector<Problem> problems;
  for (const auto &problemJson : parsed_response["result"]["problems"]) {
    Problem problem;
    factory.convert(problemJson, problem);
    problems.push_back(problem);
  }

  std::vector<ProblemStatistics> stats;
  for (const auto &statJson : parsed_response["result"]["problemStatistics"]) {
    ProblemStatistics stat;
    factory.convert(statJson, stat);
    stats.push_back(stat);
  }

  return problems;
}

std::string Cf_Controller::getProblemLink(Problem& p) {
  std::string address = "https://codeforces.com/problemset/problem/";
  std::string contestId = std::to_string((p.contestId).value());
  std::string index = p.index;

  if ((p.contestId).has_value()) {
    return address + contestId + "/" + index;
  }
  return address + index;
}

std::vector<std::string> Cf_Controller::generateRandomProblems(std::vector<std::string>& tags, size_t cnt) {
  auto pr = getProblemsByTags(tags);
  size_t sz = pr.size();
  std::vector<size_t> shuffle(sz);
  for (size_t i = 0; i < sz; i++) {
    shuffle[i] = i;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(shuffle.begin(), shuffle.end(), gen);


  std::vector<std::string> prLinks(cnt);
  for (size_t i = 0; i < cnt; i++) {
    prLinks[i] = getProblemLink(pr[shuffle[i]]);
  }

  return prLinks;
}