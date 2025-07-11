#include "utils/ProblemJsonConverter.h"
#include "entities/Object.h"
#include "entities/Problem.h"
#include <nlohmann/json.hpp>
#include <typeinfo>

void fromJson2Problem(const json &j, Problem &p) {
  if (j.contains("contestId")) {
    p.contestId = j["contestId"].get<int>();
  }

  if (j.contains("problemsetName")) {
    p.problemsetName = j["problemsetName"].get<std::string>();
  }

  j.at("index").get_to(p.index);

  j.at("name").get_to(p.name);

  std::string type;

  j.at("type").get_to(type);
  p.type = (type == "PROGRAMMING") ? Type::PROGRAMMING : Type::QUESTION;

  if (j.contains("points")) {
    p.points = j["points"].get<float>();
  }

  if (j.contains("rating")) {
    p.rating = j["rating"].get<int>();
  }

  j.at("tags").get_to(p.tags);
}

void ProblemJsonConverter::Convert(const json &Json, Object &obj) {
  if (!canHandle(obj)) {
    throw std::runtime_error("Cannot convert this object type");
  }

  Problem &problem = static_cast<Problem &>(obj);
  fromJson2Problem(Json, problem);
}

bool ProblemJsonConverter::canHandle(const Object &obj) const {
  return typeid(obj) == typeid(Problem);
}
