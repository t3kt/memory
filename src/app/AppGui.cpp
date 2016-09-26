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
      "padding": 1,
      "margin": 3,
      "text-color": "#000000",
      "text-padding": 2
    },
    "group": {
      "border-color": "#2da1e3",
      "padding": 1,
      "border-width": 1,
      "margin-left": 10,
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
//  _gui.setupFlexBoxLayout();
  auto mainPanel = _gui.addPanel();
  mainPanel->setTheme(ofJson::parse(themeText));

//  auto rootTabs = mainPanel->addTabs();
  {
    auto coreTab = mainPanel->addGroup(_appParams.core);//, ofJson{
//      {"show-header", true},
//    });
  }

//  _gui.addGroup(_appParams);

//  _pages.setup();
//  _pages.setSize(240, 700);
//  _pages.setShowHeader(false);
//  _pages.setPosition(0, 60);
//
//  _corePage.setup("Core");
//  _corePage.add(new ofxGuiGroupExtended(_appParams.core));
//  _pages.add(&_corePage);
//
//  _entityPages.setup();
//  _entityPages.setSize(230, 700);
//  _entityPages.setShowHeader(false);
//
//  _entityPage.setup();
//  _entityPage.setName("Ent");
//  _entityPage.add(&_entityPages);
//  _pages.add(&_entityPage);
//
//  _observersPage.setup("Obs");
//  auto observersGroup = new ofxGuiGroupExtended(_appParams.observers);
//  _observersPage.add(observersGroup);
//  _observersPage.setSize(230, 700);
//  observersGroup->setPosition(15, 100);
//  _entityPages.add(&_observersPage);
//
//  _occurrencesPage.setup("Occ");
//  _occurrencesPage.add(new ofxGuiGroupExtended(_appParams.occurrences));
//  _occurrencesPage.setSize(230, 700);
//  _entityPages.add(&_occurrencesPage);
//
//  _navigatorsPage.setup("Nav");
//  _navigatorsPage.add(new ofxGuiGroupExtended(_appParams.navigators));
//  _navigatorsPage.setSize(230, 700);
//  _entityPages.add(&_navigatorsPage);
//
//  _animationsPage.setup("Ani");
//  _animationsPage.add(new ofxGuiGroupExtended(_appParams.animations));
//  _pages.add(&_animationsPage);
//
//  _colorsPage.setup("Col");
//  _colorsPage.add(new ofxGuiGroupExtended(_appParams.colors));
//  _pages.add(&_colorsPage);
//
//  _renderingPage.setup("Ren");
//  _renderingPage.add(new ofxGuiGroupExtended(_appParams.rendering));
//  _pages.add(&_renderingPage);
//
//  _physicsPage.setup("Phy");
//  _physicsPage.add(new ofxGuiGroupExtended(_appParams.physics));
//  _pages.add(&_physicsPage);
//
//  _debugPage.setup("Dbg");
//  _debugPage.add(new ofxGuiGroupExtended(_appParams.debug));
//  _pages.add(&_debugPage);
//
//  _actions.setPosition(0, 0);
//
//  setBackgroundAlpha(&_pages, 0.5);
//  setBackgroundAlpha(&_entityPages, 0.7);
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
