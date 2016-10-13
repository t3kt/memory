//
//  AppGui.h
//

#ifndef AppGui_h
#define AppGui_h

#include <memory>
#include "../app/AppActions.h"
#include "../app/AppParameters.h"
#include "../core/Common.h"
#include "../core/Component.h"

class ofxGui;
class ofxGuiContainer;
class ofxGuiPanel;

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
private:
  void loadTheme();
  void addActionButtons(ofxGuiContainer* container);
  void addPresetButtons(ofxGuiContainer* container);

  MemoryAppParameters& _appParams;
  std::shared_ptr<ofxGui> _gui;
  ofxGuiPanel* _mainPanel;
};

#endif /* AppGui_h */
