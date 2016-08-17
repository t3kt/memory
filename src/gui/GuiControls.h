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
#include <map>
#include <string>
#include <utility>
#include <unordered_set>
#include "../core/Params.h"

class TGuiBuilder;

enum class GuiGroupMode {
  GROUP,
  PAGE,
  PANEL,
  TABBED_PAGES,
};

class TGuiBuilder {
public:
  TGuiBuilder(GuiGroupMode groupMode) : _groupMode(groupMode) { }

  TGuiBuilder& withSubBuilder(std::string&& key,
                              TGuiBuilder&& builder) {
    _subBuilders.emplace(std::forward<std::string>(key),
                         std::forward<TGuiBuilder>(builder));
    return *this;
  }

  TGuiBuilder& withSubBuilder(std::string&& key,
                              GuiGroupMode groupMode) {
    return withSubBuilder(std::forward<std::string>(key),
                          TGuiBuilder(groupMode));
  }

  ofxBaseGui* createControl(const TParamBase& param);
protected:
  ofxGuiGroupExtended* createGroup(const Params& params);
  ofxBaseGui* createToggle(const TParam<bool>& param);
  ofxBaseGui* createNumberSlider(const TParam<float>& param);
  ofxBaseGui* createNumberSlider(const TParam<int>& param);
  ofxBaseGui* createVectorSliders(const TParam<ofVec3f>& param);
  ofxBaseGui* createColorSliders(const TParam<ofFloatColor>& param);
  ofxBaseGui* createRangeSliders(const ValueRange<float>& params);
  ofxBaseGui* createRangeSliders(const ValueRange<int>& params);
private:
  GuiGroupMode _groupMode;
  std::map<std::string, TGuiBuilder> _subBuilders;
  std::unordered_set<std::string> _ignoreKeys;
};

#endif /* GuiControls_h */
