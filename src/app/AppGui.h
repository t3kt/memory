//
//  AppGui.h
//

#ifndef AppGui_h
#define AppGui_h

#include <memory>
#include <ofParameter.h>
#include <vector>
#include "../app/AppActions.h"
#include "../app/AppParameters.h"
#include "../core/Common.h"
#include "../core/Component.h"

class ofxGui;
class ofxGuiContainer;
class ofxGuiElement;
class ofxGuiPanel;
class ofxGuiTabs;

// Control panel GUI which modifies Params
class AppGui
: public AppActionHandler
, public NonCopyable
, public ComponentBase {
public:
  AppGui(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup() override;
  void draw() override;

  bool performAction(AppAction action) override;
  void collapseDisabled();
  void updatePresetButtons();
private:
  void loadAdvancedElements();
  void loadTheme();
  void addActionButtons(ofxGuiContainer* container);
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

#endif /* AppGui_h */
