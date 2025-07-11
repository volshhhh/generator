#include "entities/Problem.h"
#include "entities/ProblemStatistics.h"
#include "utils/JsonConverterFactory.h"
#include "utils/ProblemJsonConverter.h"
#include "utils/ProblemStatisticsJsonConverter.h"
#include <iostream>
#include <memory>

void initializeConverters() {
  auto &factory = JsonConverterFactory::getInstance();

  factory.registerConverter<Problem>(std::make_unique<ProblemJsonConverter>());
  factory.registerConverter<ProblemStatistics>(
      std::make_unique<ProblemStatisticsJsonConverter>());
}

void demonstrateUsage(const json &Json) {
  // Создаем объекты
  Problem problem;
  ProblemStatistics stats;

  auto &factory = JsonConverterFactory::getInstance();

  std::cout << "Converting Problem object..." << std::endl;
  if (factory.convert(Json, problem)) {
    std::cout << "Problem converted successfully!" << std::endl;
  }

  std::cout << "Converting ProblemStatistics object..." << std::endl;
  if (factory.convert(Json, stats)) {
    std::cout << "ProblemStatistics converted successfully!" << std::endl;
  }
}
