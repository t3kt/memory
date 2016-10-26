//
//  Params.cpp
//

#include <ofColor.h>
#include <ofVec3f.h>
#include <ofxGuiExtended.h>
#include "../control/Params.h"

using namespace _params_impl;

Json ParamTagSet::to_json() const {
  if (empty()) {
    return nullptr;
  }
  auto obj = Json::object();
  for (const auto& entry: *this) {
    obj[entry.first] = entry.second;
  }
  return obj;
}

void ParamTagSet::read_json(const Json &obj) {
  clear();
  if (obj.is_null()) {
    return;
  }
  JsonUtil::assertHasType(obj, Json::OBJECT);
  for (const auto& entry : obj.object_items()) {
    put(entry.first, JsonUtil::fromJson<bool>(entry.second));
  }
}

void TParamBase::setParentParams(const Params *parent) {
  _parent = parent;
  _tags.setParent(parent ? &(parent->tags()) : nullptr);
}

void TParamBase::readTagsField(const Json &obj) {
  _tags.read_json(obj["tags"]);
}

Json::object::value_type TParamBase::writeTagsField() const {
  return {"tags", _tags.to_json()};
}

template<>
const char* TParam<ofFloatColor>::getTypeName() const {
  return "color";
}

template<>
const char* TParam<ofVec3f>::getTypeName() const {
  return "vec3";
}

template<>
const char* TParam<std::string>::getTypeName() const {
  return "string";
}

TParamBase* Params::findKey(const std::string &key) {
  for (auto param : _paramBases) {
    if (param->getKey() == key) {
      return param;
    }
  }
  return nullptr;
}

const TParamBase* Params::findKey(const std::string &key) const {
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
  auto pos = path.find(pathSeparator);
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

void Params::performRecursiveParamAction(ParamBaseAction action) {
  for (auto param : _paramBases) {
    if (param->isGroup()) {
      auto group = dynamic_cast<Params*>(param);
      group->performRecursiveParamAction(action);
    } else {
      action(*param);
    }
  }
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

Json Params::toFilteredJson(ConstParamPredicate filter) const {
  Json::object obj;
  for (auto param : _paramBases) {
    if (filter(*param)) {
      obj.insert(param->toJsonField());
    }
  }
  return obj;
}

void Params::readFilteredJson(const Json &obj,
                              ConstParamPredicate filter) {
  for (auto param : _paramBases) {
    if (filter(*param)) {
      const auto val = obj[param->getKey()];
      if (!val.is_null()) {
        param->read_json(val);
      }
    }
  }
}

void TParamBase::linkGui(ofxGuiElement *element) {
  element->setAttribute("tparam", this);
  element->setAttribute("advanced", tags()[PTags::advanced]);
}

template<typename T>
static ofxGuiElement*
addTypedParamGuiTo(TParamBase* param,
                   ofxGuiContainer* container) {
  auto typedParam = dynamic_cast<TParam<T>*>(param);
  return container->add(*typedParam);
}

ofxGuiElement*
_params_impl::addSimpleParamGuiTo(TParamBase *param,
                                  ofxGuiContainer *container) {
  const auto& type = param->getTypeInfo();
  if (type == typeid(bool)) {
    auto element = addTypedParamGuiTo<bool>(param, container);
    static_cast<ofxGuiToggle*>(element)->setType(ofxGuiToggleType::FULLSIZE);
    return element;
  } else if (type == typeid(float)) {
    return addTypedParamGuiTo<float>(param, container);
  } else if (type == typeid(int)) {
    return addTypedParamGuiTo<int>(param, container);
  } else if (type == typeid(ofVec3f)) {
    return addTypedParamGuiTo<ofVec3f>(param, container);
  } else if (type == typeid(ofFloatColor)) {
    return addTypedParamGuiTo<ofFloatColor>(param, container);
  } else if (type == typeid(std::string)) {
    return addTypedParamGuiTo<std::string>(param, container);
  } else {
    return nullptr;
  }
}

ofxGuiElement* Params::addGuiTo(ofxGuiContainer *container) {
  return addGroupGuiTo(container, GuiGroupType::GROUP);
}

ofxGuiContainer*
Params::addEmptyGroupTo(ofxGuiContainer *container,
                        GuiGroupType groupType) {
  switch (groupType) {
    case GuiGroupType::TABS:
      return container->addTabs(getName());
    case GuiGroupType::GROUP:
    default:
      return container->addGroup(getName());
  }
}

ofxGuiElement* Params::addGroupGuiTo(ofxGuiContainer *container,
                                     GuiGroupType groupType) {
  auto group = addEmptyGroupTo(container, groupType);
  addChildrenTo(group);
  TParamBase::linkGui(group);
  return group;
}

void Params::addChildrenTo(ofxGuiContainer *group) {
  for (auto child : getParamBases()) {
    child->addGuiTo(group);
  }
}

class AdditionalParamsContainer
: public ofxGuiGroup {
public:
  AdditionalParamsContainer(ParamsWithEnabled& params)
  : _params(params) {
    setShowHeader(false);
    params.enabled.changed.addVoidListener([&]() {
      updateEnabled();
    }, this);
  }
private:
  void updateEnabled() {
    if (_params.enabled.get()) {
      maximize();
    } else {
      minimize();
    }
  }

  ParamsWithEnabled& _params;
};

void ParamsWithEnabled::addChildrenTo(ofxGuiContainer *group) {
  enabled.addGuiTo(group);
  auto otherParamsContainer =
    group->add<AdditionalParamsContainer>(*this);
  for (auto child : getParamBases()) {
    if (child == &enabled) {
      continue;
    }
    child->addGuiTo(otherParamsContainer);
  }
}
