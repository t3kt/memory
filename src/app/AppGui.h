//
//  AppGui.h
//

#ifndef AppGui_h
#define AppGui_h

#include <memory>
#include "../app/AppActions.h"
#include "../app/AppParameters.h"

class ofxGui;
class ofxGuiPanel;

// Control panel GUI which modifies Params
class AppGui
: public AppActionHandler {
public:
  AppGui(MemoryAppParameters& appParams,
         AppActionHandler& actionHandler)
  : _appParams(appParams)
  , _actionHandler(actionHandler) { }

  void setup();
  void draw();

  bool performAction(AppAction action) override;
private:
  void loadTheme();
//  void onLoad() {
//    _actionHandler.performAction(AppAction::LOAD_SETTINGS);
//  }
//
//  void onSave() {
//    _actionHandler.performAction(AppAction::SAVE_SETTINGS);
//  }

  MemoryAppParameters& _appParams;
  AppActionHandler& _actionHandler;
  std::shared_ptr<ofxGui> _gui;
  ofxGuiPanel* _mainPanel;
};

#endif /* AppGui_h */
