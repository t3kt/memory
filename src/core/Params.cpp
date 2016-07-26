//
//  Params.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Params.h"

TParamBase* Params::findKey(const std::string &key) {
  for (auto param : _paramBases) {
    if (param->getKey() == key) {
      return param;
    }
  }
  return nullptr;
}

TParamBase* Params::lookupPath(const std::string &path) {
  if (path.empty()) {
    return nullptr;
  }
  auto pos = path.find('.');
  if (pos == std::string::npos) {
    return findKey(path);
  }
  auto key = path.substr(0, pos);

  TParamBase* param = findKey(key);
  if (!param || !param->isGroup()) {
    return nullptr;
  }
  Params* group = dynamic_cast<Params*>(param);
  auto subpath = path.substr(pos + 1);
  return group->lookupPath(subpath);
}

void Params::readJsonField(const Json& obj) {
  const Json& val = obj[getKey()];
  if (!val.is_null()) {
    JsonUtil::assertHasType(val, Json::OBJECT);
    read_json(val);
  } else if (hasDefault()) {
    resetToDefault();
  } else {
    throw JsonException("Required field missing: " + getKey());
  }
}

Json Params::to_json() const {
  Json::object obj;
  for (auto param : _paramBases) {
    obj.insert(param->toJsonField());
  }
  return obj;
}

void Params::read_json(const Json &val) {
  JsonUtil::assertHasType(val, Json::OBJECT);
  for (auto param : _paramBases) {
    param->readJsonField(val);
  }
}
