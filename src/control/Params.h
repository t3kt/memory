//
//  Params.h
//

#pragma once

#include <ofParameterGroup.h>
#include <ofUtils.h>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <ofxTagSet.h>
#include <ofxTEvents.h>
#include <string>
#include <typeinfo>
#include <unordered_set>
#include <vector>

namespace PTags {
  const std::string osc = "osc";
  const std::string preset = "preset";
  const std::string advanced = "advanced";
}

namespace _params_impl {

  class ParamTagSet
  : public ofxTagSet<std::string>
  , public ofxTCommon::JsonReadable
  , public ofxTCommon::JsonWritable {
  public:
    ParamTagSet() {}

    ParamTagSet(std::initializer_list<std::string> tags)
    : ofxTagSet<std::string>(tags) { }

    ofJson toJson() const override;
    void readJson(const ofJson& obj) override;
  };

}

class ofxGuiContainer;
class ofxGuiElement;
class Params;

class TParamBase
: public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  using ParamTagSet = _params_impl::ParamTagSet;
  static const char pathSeparator = '.';

  TParamBase() { }

  const Params* getParentParams() const { return _parent; }

  void setParentParams(const Params* parent);

  const std::string& getKey() const {
    return _key;
  }

  ofJson::object_t::value_type toJsonField() const;
  virtual void readJsonField(const ofJson& obj) = 0;
  virtual ofJson toJsonMetadata() const = 0;

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
  void readTagsField(const ofJson& obj);
  ofJson::object_t::value_type writeTagsField() const;

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

    ofJson toJson() const override {
      return ofxTCommon::JsonUtil::toJson(ofParameter<T>::get());
    }

    void readJson(const ofJson& val) override {
      this->set(ofxTCommon::JsonUtil::fromJson<T>(val));
    }

    void readJsonField(const ofJson& obj) override {
      if (obj.count(getKey()) != 0) {
        auto val = obj[getKey()];
        if (!val.is_null()) {
          readJson(val);
          return;
        }
      }
      if (hasDefault()) {
        ofParameter<T>::set(getDefaultValue());
      } else {
        throw ofxTCommon::JsonException("Required field missing: " + getKey());
      }
    }

    ofJson toJsonMetadata() const override {
      ofJson metadata = {
        {"key", getKey()},
        {"name", ofParameter<T>::getName()},
        {"type", getTypeName()},
        {"min", ofxTCommon::JsonUtil::toJson(ofParameter<T>::getMin())},
        {"max", ofxTCommon::JsonUtil::toJson(ofParameter<T>::getMax())},
      };
      if (hasDefault()) {
        metadata["default"] = ofxTCommon::JsonUtil::toJson(getDefaultValue());
      }
      if (hasTags()) {
        metadata.push_back(writeTagsField());
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
    ofParameter<T>::set(ofxTCommon::getInterpolated(ofParameter<T>::getMin(),
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
  , public ofxTCommon::NonCopyable {
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

  ofJson toJson() const override;

  ofJson toFilteredJson(ConstParamPredicate filter) const;

  void readJson(const ofJson& val) override;

  void readJsonField(const ofJson& obj) override;

  void readFilteredJson(const ofJson& obj, ConstParamPredicate filter);

  ofJson toJsonMetadata() const override {
    auto paramMetas = ofJson::array();
    for (const auto param : _paramBases) {
      paramMetas.push_back(param->toJsonMetadata());
    }
    ofJson metadata = {
      {"key", getKey()},
      {"name", ofParameterGroup::getName()},
      {"params", paramMetas},
    };
    if (hasTags()) {
      metadata.push_back(writeTagsField());
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

