#pragma once

#include "JsonConverter.h"
#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ProblemJsonConverter.h"
#include "ProblemStatisticsJsonConverter.h"
#include "entities/Problem.h"
#include "entities/ProblemStatistics.h"

class JsonConverterFactory {
public:
  JsonConverterFactory() = default;

  template <typename T>
  void registerConverter(std::unique_ptr<JsonConverter> converter) {
    converters_[std::type_index(typeid(T))] = std::move(converter);
  }

  static JsonConverterFactory &getInstance();

  void initConverters();

  JsonConverter *getConverter(const Object &obj);

  bool convert(const json &Json, Object &obj);

private:
  std::unordered_map<std::type_index, std::unique_ptr<JsonConverter>>
      converters_;
};
