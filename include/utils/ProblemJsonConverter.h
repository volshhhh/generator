#pragma once

#include "JsonConverter.h"

class ProblemJsonConverter : public JsonConverter {
public:
  void Convert(const json &Json, Object &obj) override;
  bool canHandle(const Object &obj) const override;
};
