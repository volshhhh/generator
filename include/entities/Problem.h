#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Object.h"
#include "Type.h"

struct Problem : public Object {
  std::optional<int> contestId;

  std::optional<std::string> problemsetName;

  std::string index;

  std::string name;

  Type type;

  std::optional<float> points;

  std::optional<int> rating;

  std::vector<std::string> tags;
};