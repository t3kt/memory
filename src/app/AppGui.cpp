//
//  AppGui.cpp
//

#include <ofxGuiExtended.h>
#include "../app/AppGui.h"

//class ActionsGui
//: public ofxGuiGroup {
//public:
//  ActionsGui(AppActionHandler& actionHandler)
//  : _actionHandler(actionHandler) {
//    setup();
//    setName("Actions");
//
//    auto loadButton = add<ofxGuiButton>("load");
//    loadButton->addListener(this, &ActionsGui::onLoad);
//
//    auto saveButton = add<ofxGuiButton>("save");
//    saveButton->addListener(this, &ActionsGui::onSave);
//
//    minimize();
//  }
//private:
//  void onLoad() {
//    _actionHandler.performAction(AppAction::LOAD_SETTINGS);
//  }
//
//  void onSave() {
//    _actionHandler.performAction(AppAction::SAVE_SETTINGS);
//  }
//
//  AppActionHandler& _actionHandler;
//};

class AppGuiImpl {
public:
  AppGuiImpl(MemoryAppParameters& appParams,
             AppActionHandler& actionHandler)
  : _appParams(appParams)
  , _actionHandler(actionHandler) { }

  void setup();

  void draw() {
//    _pages.draw();
//    _actions.draw();
  }
private:
  void onLoad() {
    _actionHandler.performAction(AppAction::LOAD_SETTINGS);
  }

  void onSave() {
    _actionHandler.performAction(AppAction::SAVE_SETTINGS);
  }

  MemoryAppParameters& _appParams;
  AppActionHandler& _actionHandler;

//  ofxGuiPage _corePage;
//  ofxGuiPage _entityPage;
//  ofxGuiPage _animationsPage;
//  ofxGuiPage _colorsPage;
//  ofxGuiPage _renderingPage;
//  ofxGuiPage _physicsPage;
//  ofxGuiPage _debugPage;
//
//  ofxTabbedPages _pages;
//
//  ofxGuiPage _observersPage;
//  ofxGuiPage _occurrencesPage;
//  ofxGuiPage _navigatorsPage;
//
//  ofxTabbedPages _entityPages;

  ofxGui _gui;
};

#define _STRINGIFY(A) #A

const std::string themeText =
R""(
{
  "default": {
    "base": {
      "background-color": "rgba(255,255,255,0.4)",
      "fill-color": "rgba(255,255,255,0.8)",
      "border-width": 0,
      "padding": 0.5,
      "margin": 1,
      "text-color": "#000000",
      "text-padding": 1
    },
    "group": {
      "border-color": "#2da1e3",
      "padding": 1,
      "border-width": 1,
      "margin-left": 5,
      "show-header": true
    },
    "group-header": {
      "background-color": "#2da1e3",
      "text-color": "#ffffff"
    },
    "panel": {
      "background-color": "#FFFFFF"
    }
  }
}
)"";

void AppGuiImpl::setup() {
  _gui.setupFlexBoxLayout();
  auto mainPanel = _gui.addPanel();
  mainPanel->setBackgroundColor(ofFloatColor(1, 1, 1, 0.5));
  auto rootGroup = mainPanel->addGroup();

  auto rootTabs = rootGroup->addTabs();
  rootTabs->setTabHeight(6);
  rootTabs->setTabWidth(44);
  auto coreTab = rootTabs->addGroup(_appParams.core);
  {
    auto entityTabs = rootTabs->addTabs("Ent");
    entityTabs->setTabHeight(6);
    entityTabs->setTabWidth(44);
    entityTabs->addGroup(_appParams.observers)->setName("Obs");
    entityTabs->addGroup(_appParams.occurrences)->setName("Occ");
    entityTabs->addGroup(_appParams.navigators)->setName("Nav");
  }
  rootTabs->addGroup(_appParams.animations)->setName("Ani");
  rootTabs->addGroup(_appParams.colors)->setName("Col");
  rootTabs->addGroup(_appParams.rendering)->setName("Ren");
  rootTabs->addGroup(_appParams.physics)->setName("Phys");
  rootTabs->addGroup(_appParams.debug)->setName("Dbg");

  mainPanel->setTheme(ofJson::parse(themeText));
}

AppGui::AppGui(MemoryAppParameters& appParams,
               AppActionHandler& actionHandler)
: _impl(new AppGuiImpl(appParams, actionHandler))
{
  _impl->setup();
}

void AppGui::draw() {
//  _impl->draw();
}
