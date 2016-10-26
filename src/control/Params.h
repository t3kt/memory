//
//  Params.h
//

#pragma once

#include <ofParameterGroup.h>
#include <ofUtils.h>
#include <ofxTagSet.h>
#include <ofxTEvents.h>
#include <string>
#include <typeinfo>
#include <unordered_set>
#include <vector>
#include "../core/Common.h"
#include "../core/JsonIO.h"

namespace PTags {
  const std::string osc = "osc";
  const std::string preset = "preset";
  const std::string advanced = "advanced";
}

namespace _params_impl {

  class ParamTagSet
  : public ofxTagSet<std::string>
  , public JsonReadable
  , public JsonWritable {
  public:
    ParamTagSet() {}

    ParamTagSet(std::initializer_list<std::string> tags)
    : ofxTagSet<std::string>(tags) { }

    Json to_json() const override;
    void read_json(const Json& obj) override;
  };

}

class ofxGuiContainer;
class ofxGuiElement;
class Params;

class TParamBase
: public JsonReadable
, public JsonWritable {
public:
  using ParamTagSet = _params_impl::ParamTagSet;
  static const char pathSeparator = '.';

  TParamBase() { }

  const Params* getParentParams() const { return _parent; }

  void setParentParams(const Params* parent);

  const std::string& getKey() const {
    return _key;
  }

  virtual Json::object::value_type toJsonField() const {
    return { getKey(), to_json() };
  }
  virtual Json toJsonMetadata() const = 0;
  virtual void readJsonField(const Json& obj) = 0;
  virtual void resetToDefault() = 0;
  virtual bool hasDefault() const = 0;
  virtual bool isGroup() const = 0;

  virtual std::string asString() const = 0;
  virtual const std::type_info& getTypeInfo() const = 0;
  virtual const char* getTypeName() const = 0;
  bool supportsOsc() const { return tags()[PTags::osc]; }
  bool supportsPresets() const { return tags()[PTags::preset]; }

  ParamTagSet& tags() { return _tags; }
  const ParamTagSet& tags() const { return _tags; }
  bool hasTags() const { return !_tags.empty(); }

  virtual ofxGuiElement* addGuiTo(ofxGuiContainer* container) = 0;
protected:
  void readTagsField(const Json& obj);
  Json::object::value_type writeTagsField() const;

  void linkGui(ofxGuiElement* element);

  std::string _key;
  ParamTagSet _tags;
  const Params* _parent;
};

namespace _params_impl {

  ofxGuiElement* addSimpleParamGuiTo(TParamBase* param,
                                     ofxGuiContainer* container);

  template<typename ParT>
  class TParamBaseWithInitializers
  : public TParamBase {
  public:

    ParT& setKey(std::string key) {
      TParamBase::_key = key;
      return selfRef();
    }

    ParT& setSupportsOsc(bool support) {
      return withTagSetTo(PTags::osc, support);
    }

    ParT& setSupportsPresets(bool support) {
      return withTagSetTo(PTags::preset, support);
    }

    ParT& setAdvanced(bool advanced) {
      return withTagSetTo(PTags::advanced, advanced);
    }

    template<typename Arg>
    ParT& withTag(Arg tag) {
      tags().enable(tag);
      return selfRef();
    }

    template<typename Arg>
    ParT& withoutTag(Arg tag) {
      tags().enable(tag);
      return selfRef();
    }
  protected:
    template<typename Arg>
    ParT& withTagSetTo(Arg tag, bool value) {
      tags().put(tag, value);
      return selfRef();
    }

    virtual ParT& selfRef() = 0;
  };

  template<typename T, typename ParT>
  class TTypedParamBase
  : public ofParameter<T>
  , public TParamBaseWithInitializers<ParT> {
  public:
    using BaseT = TParamBaseWithInitializers<ParT>;

    TTypedParamBase() {
      ofParameter<T>::addListener(this,
                                  &TTypedParamBase::onChanged);
    }

    virtual ~TTypedParamBase() override {
      ofParameter<T>::removeListener(this,
                                     &TTypedParamBase::onChanged);
    }

    ParT& setName(std::string name) {
      ofParameter<T>::setName(name);
      return selfRef();
    }

    ParT& setRange(T minValue, T maxValue) {
      ofParameter<T>::setMin(minValue);
      ofParameter<T>::setMax(maxValue);
      return selfRef();
    }

    ParT& setDefaultValue(T defaultValue) {
      _defaultValue = defaultValue;
      _hasDefaultValue = true;
      return selfRef();
    }

    ParT& setValueAndDefault(T value) {
      ofParameter<T>::set(value);
      return setDefaultValue(value);
    }

    const T& operator()() const {
      return ofParameter<T>::get();
    }

    bool hasDefault() const override {
      return _hasDefaultValue;
    }

    const T& getDefaultValue() const {
      return _defaultValue;
    }

    void clearDefaultValue() {
      _defaultValue = {};
      _hasDefaultValue = false;
    }

    void resetToDefault() override {
      if (hasDefault()) {
        ofParameter<T>::set(getDefaultValue());
      }
    }

    using BaseT::getKey;

    bool isGroup() const override { return false; }

    const std::type_info& getTypeInfo() const override {
      return typeid(T);
    }

    std::string asString() const override { return ofParameter<T>::toString(); }

    ofxTCommon::TEvent<T&> changed;

    Json to_json() const override {
      return JsonUtil::toJson(ofParameter<T>::get());
    }

    void read_json(const Json& val) override {
      this->set(JsonUtil::fromJson<T>(val));
    }

    void readJsonField(const Json& obj) override {
      Json val = obj[getKey()];
      if (!val.is_null()) {
        read_json(val);
      } else if (hasDefault()) {
        ofParameter<T>::set(getDefaultValue());
      } else {
        throw JsonException("Required field missing: " + getKey());
      }
    }

    Json toJsonMetadata() const override {
      auto metadata = Json::object {
        {"key", getKey()},
        {"name", ofParameter<T>::getName()},
        {"type", getTypeName()},
        {"min", JsonUtil::toJson(ofParameter<T>::getMin())},
        {"max", JsonUtil::toJson(ofParameter<T>::getMax())},
      };
      if (hasDefault()) {
        metadata["default"] = JsonUtil::toJson(getDefaultValue());
      }
      if (hasTags()) {
        metadata.insert(writeTagsField());
      }
      return metadata;
    }

    ofxGuiElement* addGuiTo(ofxGuiContainer* container) override {
      auto element = _params_impl::addSimpleParamGuiTo(this, container);
      TParamBase::linkGui(element);
      return element;
    }

    using BaseT::getTypeName;
    using BaseT::hasTags;
    
  protected:

    using BaseT::selfRef;
    using BaseT::writeTagsField;
    using BaseT::readTagsField;

  private:
    void onChanged(T& value) {
      changed.notifyListeners(value);
    }

    T _defaultValue;
    bool _hasDefaultValue;
  };
  
}

template<typename T>
class TParam
: public _params_impl::TTypedParamBase<T, TParam<T>> {
public:

  void setNormalizedValue(float normVal) {
    ofParameter<T>::set(getInterpolated(ofParameter<T>::getMin(),
                                        ofParameter<T>::getMax(),
                                        normVal));
  }

  float getNormalizedValue() const {
    return ofMap(ofParameter<T>::get(),
                 ofParameter<T>::getMin(),
                 ofParameter<T>::getMax(),
                 0.0f,
                 1.0f);
  }

  const char* getTypeName() const override {
    return _params_impl::TTypedParamBase<T, TParam<T>>::getTypeInfo().name();
  }

protected:

  TParam<T>& selfRef() override { return *this; }
};

template<>
class TParam<bool>
: public _params_impl::TTypedParamBase<bool, TParam<bool>> {
public:

  void setNormalizedValue(float normVal) {
    ofParameter<bool>::set(normVal > 0);
  }

  float getNormalizedValue() const {
    return ofParameter<bool>::get() ? 1.0f : 0.0f;
  }

  void toggle() {
    ofParameter<bool>::set(!ofParameter<bool>::get());
  }

  const char* getTypeName() const override {
    return TTypedParamBase<bool, TParam<bool>>::getTypeInfo().name();
  }

protected:

  TParam<bool>& selfRef() override { return *this; }
};

using ParamBaseAction = std::function<void(TParamBase&)>;
using ConstParamPredicate = std::function<bool(const TParamBase&)>;

class Params
: public ofParameterGroup
, public _params_impl::TParamBaseWithInitializers<Params>
, public NonCopyable {
public:
  enum class GuiGroupType {
    GROUP,
    TABS,
  };

  static ConstParamPredicate filterByTag(const std::string& tag) {
    return [&](const TParamBase& param) {
      return param.tags()[tag];
    };
  }

  using BaseT = _params_impl::TParamBaseWithInitializers<Params>;

  Params() {}
  Params(std::string key, std::string label) {
    setKey(key);
    setName(label);
  }

  Params& setName(std::string name) {
    ofParameterGroup::setName(name);
    return selfRef();
  }

  using BaseT::setAdvanced;
  using BaseT::getKey;

  template<typename P>
  void add(P& param) {
    param.setParentParams(this);
    ofParameterGroup::add(param);
    _paramBases.push_back(&param);
  }

  Json to_json() const override;

  Json toFilteredJson(ConstParamPredicate filter) const;

  void read_json(const Json& val) override;

  void readJsonField(const Json& obj) override;

  void readFilteredJson(const Json& obj, ConstParamPredicate filter);

  Json toJsonMetadata() const override {
    auto paramMetas = Json::array {};
    for (const auto param : _paramBases) {
      paramMetas.push_back(param->toJsonMetadata());
    }
    auto metadata = Json::object {
      {"key", getKey()},
      {"name", ofParameterGroup::getName()},
      {"params", paramMetas},
    };
    if (hasTags()) {
      metadata.insert(writeTagsField());
    }
    return metadata;
  }

  virtual void resetToDefault() override {
    for (auto param : _paramBases) {
      param->resetToDefault();
    }
  }

  virtual bool hasDefault() const override { return true; }

  bool isGroup() const override { return true; }
  const std::type_info& getTypeInfo() const override {
    return typeid(Params);
  }

  const char* getTypeName() const override {
    return "group";
  }

  std::string asString() const override { return ofParameterGroup::toString(); }

  TParamBase* findKey(const std::string& key);
  const TParamBase* findKey(const std::string& key) const;

  TParamBase* lookupPath(const std::string& path);

  std::vector<TParamBase*>& getParamBases() { return _paramBases; }

  const std::vector<TParamBase*>& getParamBases() const {
    return _paramBases;
  }

  void performRecursiveParamAction(ParamBaseAction action);

  ofxGuiElement* addGuiTo(ofxGuiContainer* container) override;

  virtual ofxGuiElement* addGroupGuiTo(ofxGuiContainer* container,
                                       GuiGroupType groupType);

protected:
  ofxGuiContainer* addEmptyGroupTo(ofxGuiContainer* container,
                                   GuiGroupType groupType);
  virtual void addChildrenTo(ofxGuiContainer* group);
  

  Params& selfRef() override { return *this; }

private:
  std::vector<TParamBase*> _paramBases;
};

class ParamsWithEnabled : public Params {
public:
  ParamsWithEnabled() {
    add(enabled
        .setKey("enabled")
        .setName("Enabled")
        .setValueAndDefault(true));
  }

  void setEnabledValueAndDefault(bool val) {
    enabled.setValueAndDefault(val);
  }

  void addChildrenTo(ofxGuiContainer* group) override;

  TParam<bool> enabled;
};

