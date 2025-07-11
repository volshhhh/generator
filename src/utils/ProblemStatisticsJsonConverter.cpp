#include "utils/ProblemStatisticsJsonConverter.h"
#include "entities/Object.h"
#include "entities/ProblemStatistics.h"
#include <nlohmann/json.hpp>
#include <typeinfo>

void fromJson2ProblemStatistics(const json &j, ProblemStatistics &ps) {
  if (j.contains("contestId")) {
    ps.contestId = j["contestId"].get<int>();
  }

  j.at("index").get_to(ps.index);

  j.at("solvedCount").get_to(ps.solvedCount);
}

void ProblemStatisticsJsonConverter::Convert(const json &Json, Object &obj) {
  if (!canHandle(obj)) {
    throw std::runtime_error("Cannot convert this object type");
  }

  ProblemStatistics &ps = static_cast<ProblemStatistics &>(obj);
  fromJson2ProblemStatistics(Json, ps);
}

bool ProblemStatisticsJsonConverter::canHandle(const Object &obj) const {
  return typeid(obj) == typeid(ProblemStatistics);
}