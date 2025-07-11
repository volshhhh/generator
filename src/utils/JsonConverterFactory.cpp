#include "utils/JsonConverterFactory.h"

using json = nlohmann::json;

JsonConverterFactory &JsonConverterFactory::getInstance() {
  static JsonConverterFactory instance;
  return instance;
}

void JsonConverterFactory::initConverters() {
  registerConverter<Problem>(std::make_unique<ProblemJsonConverter>());
  registerConverter<ProblemStatistics>(
      std::make_unique<ProblemStatisticsJsonConverter>());
}

JsonConverter *JsonConverterFactory::getConverter(const Object &obj) {
  auto it = converters_.find(std::type_index(typeid(obj)));
  if (it != converters_.end()) {
    return it->second.get();
  }
  return nullptr;
}

bool JsonConverterFactory::convert(const json &Json, Object &obj) {
  JsonConverter *converter = getConverter(obj);
  if (converter) {
    converter->Convert(Json, obj);
    return true;
  }

  std::cerr << "No converter found for type: " << typeid(obj).name()
            << std::endl;
  return false;
}
