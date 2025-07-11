#pragma once

#include "Object.h"
#include <optional>
#include <string>

struct ProblemStatistics : public Object {
  std::optional<int> contestId;
  std::string index;
  int solvedCount;
};