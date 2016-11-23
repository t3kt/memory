//
//  AppGui.h
//

#pragma once

#include <memory>
#include <ofParameter.h>
#include <ofxTCommon.h>
#include <vector>
#include "../app/AppParameters.h"
#include "../core/Component.h"

class ofxGui;
class ofxGuiContainer;
class ofxGuiElement;
class ofxGuiPanel;
class ofxGuiTabs;

// Control panel GUI which modifies Params
class AppGui
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  AppGui(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup() override;
  void draw() override;

  void collapseDisabled();
  void updatePresetButtons();
private:
  void loadAdvancedElements();
  void loadTheme();
  void addCommandButtons(ofxGuiContainer* container);
  void addPresetButtons(ofxGuiContainer* container);
  void onShowAdvancedChanged(bool& visible);
  void setAdvancedHidden(bool hidden);

  MemoryAppParameters& _appParams;
  std::shared_ptr<ofxGui> _gui;
  ofxGuiPanel* _mainPanel;
  ofxGuiTabs* _rootTabs;
  ofxGuiContainer* _presetsContainer;
  ofParameter<bool> _showAdvanced;
  std::vector<ofxGuiElement*> _advancedElements;
};

