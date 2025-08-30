#pragma once

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "entities/Problem.h"
#include "utils/JsonConverterFactory.h"

using json = nlohmann::json;

/*
  управляет взаимодействием
  с Codeforces API
*/
class Cf_Controller {
private:
  const std::string api_address = "https://codeforces.com/api/";
  CURL *curl;

  // Callback функция для обработки полученных данных
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *output) {
    const char *data = static_cast<char *>(contents);
    size_t data_size = size * nmemb;
    if (data_size > 1024 * 1024) {
      return 0;
    }
    output->append(data, data_size);
    return data_size;
  };

  // получаем задачи по тегам
  std::vector<Problem> getProblemsByTags(std::vector<std::string> &tags);

  // получает ссылку на задачу
  std::string getProblemLink(Problem &p);

public:
  Cf_Controller();
  ~Cf_Controller();

  // возвращает ссылки задачи, выбранные рандомно
  std::vector<std::string> generateRandomProblems(std::vector<std::string> &tags,
    size_t cnt);
};