//
//  GuiControls.h
//  memory
//
//  Created by tekt on 8/16/16.
//
//

#ifndef GuiControls_h
#define GuiControls_h

#include <ofTypes.h>
#include <ofxGuiExtended.h>
#include <memory>
#include <map>
#include <string>
#include <type_traits>
#include <utility>
#include <unordered_set>
#include "../core/Params.h"

class TGuiBuilder;

enum class GuiGroupMode {
  GROUP,
  PAGE,
  PAGE_WITH_INNER_GROUP,
  PANEL,
  TABBED_PAGES,
};

enum class GuiSliderStyle {
  ROTARY_SLIDER,
  SLIDER,
  VERTICAL_SLIDER,
};

class TGuiBuilder;
using TGuiBuilderPtr = std::shared_ptr<TGuiBuilder>;

class TGuiBuilder : public std::enable_shared_from_this<TGuiBuilder> {
public:
  using ChainReturnT = TGuiBuilderPtr;

  TGuiBuilder(GuiGroupMode groupMode)
  : _groupMode(groupMode)
  , _defaultSliderStyle(GuiSliderStyle::SLIDER)
  //  , _width(200)
  { }

  TGuiBuilder()
  : _groupMode(GuiGroupMode::GROUP)
  , _defaultSliderStyle(GuiSliderStyle::SLIDER)
//  , _width(200)
  { }

  ChainReturnT withGroupMode(GuiGroupMode groupMode) {
    _groupMode = groupMode;
    return chainRet();
  }

//  ChainReturnT withWidth(int width) {
//    _width = width;
//    return chainRet();
//  }

  ChainReturnT withSubBuilder(std::string key,
                              TGuiBuilderPtr builder) {
    _subBuilders.emplace(key, builder);
    return chainRet();
  }

  ChainReturnT withSubBuilder(std::string key,
                              GuiGroupMode groupMode) {
    return withSubBuilder(key,
                          std::make_shared<TGuiBuilder>()
                          ->withGroupMode(groupMode));
  }

  ChainReturnT withDefaultSubBuilder(TGuiBuilderPtr builder) {
    _defaultSubBuilder = builder;
    return chainRet();
  }

  ChainReturnT withSliderStyle(std::string key, GuiSliderStyle style) {
    _sliderStyles.emplace(key, style);
    return chainRet();
  }

  ChainReturnT withDefaultSliderStyle(GuiSliderStyle style) {
    _defaultSliderStyle = style;
    return chainRet();
  }

  ofxBaseGui* createControl(const TParamBase& param,
                            bool isChild = false) const;
protected:
  ofxBaseGui* createGroup(const Params& params) const;
  ofxBaseGui* createChildGroup(const Params& params) const;
  ofxBaseGui* createToggle(const TParam<bool>& param) const;
  ofxBaseGui* createVectorSliders(const TParam<ofVec3f>& param) const;
  ofxBaseGui* createColorSliders(const TParam<ofFloatColor>& param) const;
  ofxBaseGui* createRangeSliders(const ValueRange<float>& params) const;
  ofxBaseGui* createRangeSliders(const ValueRange<int>& params) const;

  template<typename T>
  ofxBaseGui* createNumberSlider(const TParam<T>& param) const;

  GuiSliderStyle getSliderStyle(const TParamBase& param) const {
    auto iter = _sliderStyles.find(param.getKey());
    if (iter != _sliderStyles.end()) {
      return iter->second;
    }
    return _defaultSliderStyle;
  }

  const TGuiBuilder& getSubBuilder(const TParamBase& params) const {
    auto iter = _subBuilders.find(params.getKey());
    if (iter != _subBuilders.end()) {
      return *(iter->second);
    } else if (_defaultSubBuilder) {
      return *_defaultSubBuilder;
    } else {
      return *this;
    }
  }

  void populateGroup(ofxGuiGroupExtended* group,
                     const Params& params) const;
private:
  inline ChainReturnT chainRet() { return std::shared_ptr<TGuiBuilder>(this); }

  GuiGroupMode _groupMode;
  std::map<std::string, TGuiBuilderPtr> _subBuilders;
  TGuiBuilderPtr _defaultSubBuilder;
  std::map<std::string, GuiSliderStyle> _sliderStyles;
  GuiSliderStyle _defaultSliderStyle;
  std::unordered_set<std::string> _ignoreKeys;
//  int _width;
};

#endif /* GuiControls_h */
