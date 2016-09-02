//
//  AppGui.h
//

#ifndef AppGui_h
#define AppGui_h

#include <memory>
#include <ofxGuiExtended.h>
#include "../app/AppActions.h"
#include "../app/AppParameters.h"

class AppGuiImpl;

class ActionsGui
: public ofxPanelExtended {
public:
  ActionsGui(AppActionHandler& actionHandler)
  : _actionHandler(actionHandler) {
    setup();
    setName("Actions");
    _group.setup();
    add(&_group);

    auto loadButton = new ofxMinimalButton("load");
    loadButton->addListener(this, &ActionsGui::onLoad);
    _group.add(loadButton);

    auto saveButton = new ofxMinimalButton("save");
    saveButton->addListener(this, &ActionsGui::onSave);
    _group.add(saveButton);

    _group.minimize();
  }
private:
  void onLoad() {
    _actionHandler.performAction(AppAction::LOAD_SETTINGS);
  }

  void onSave() {
    _actionHandler.performAction(AppAction::SAVE_SETTINGS);
  }

  AppActionHandler& _actionHandler;
  ofxGuiGroupExtended _group;
};

// Control panel GUI which modifies Params
class AppGui {
public:
  AppGui(MemoryAppParameters& appParams);

  void setup(AppActionHandler& actionHandler);
  void draw();
private:
  MemoryAppParameters& _appParams;

  ofxGuiPage* _corePage;
  ofxGuiPage* _entityPage;
  ofxGuiPage* _animationsPage;
  ofxGuiPage* _colorsPage;
  ofxGuiPage* _renderingPage;
  ofxGuiPage* _physicsPage;

  ofxTabbedPages* _pages;

  ofxGuiPage* _observersPage;
  ofxGuiPage* _occurrencesPage;
  ofxGuiPage* _navigatorsPage;

  ofxTabbedPages* _entityPages;

  ActionsGui* _actions;
};

#endif /* AppGui_h */
