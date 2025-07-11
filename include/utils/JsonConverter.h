#pragma once

#include "entities/Object.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class JsonConverter {
public:
  virtual ~JsonConverter() = 0;
  virtual void Convert(const json &Json, Object &obj) = 0;
  virtual bool canHandle(const Object &obj) const = 0;
};

inline JsonConverter::~JsonConverter() {}