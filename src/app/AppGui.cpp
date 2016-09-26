//
//  AppGui.cpp
//

#include <ofxGuiExtended.h>
#include "../app/AppGui.h"

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

class AppGuiImpl {
public:
  AppGuiImpl(MemoryAppParameters& appParams,
             AppActionHandler& actionHandler);

  void draw() {
    _pages.draw();
    _actions.draw();
  }
private:
  MemoryAppParameters& _appParams;

  ofxGuiPage _corePage;
  ofxGuiPage _entityPage;
  ofxGuiPage _animationsPage;
  ofxGuiPage _colorsPage;
  ofxGuiPage _renderingPage;
  ofxGuiPage _physicsPage;
  ofxGuiPage _debugPage;

  ofxTabbedPages _pages;

  ofxGuiPage _observersPage;
  ofxGuiPage _occurrencesPage;
  ofxGuiPage _navigatorsPage;

  ofxTabbedPages _entityPages;

  ActionsGui _actions;
};

static void setBackgroundAlpha(ofxBaseGui* gui, float alpha) {
  gui->setBackgroundColor(ofColor(gui->getBackgroundColor(), alpha*255));
}

AppGuiImpl::AppGuiImpl(MemoryAppParameters& appParams,
                       AppActionHandler& actionHandler)
: _appParams(appParams)
, _actions(actionHandler) {
  _pages.setup();
  _pages.setSize(240, 700);
  _pages.setShowHeader(false);
  _pages.setPosition(0, 60);

  _corePage.setup("Core");
  _corePage.add(new ofxGuiGroupExtended(_appParams.core));
  _pages.add(&_corePage);

  _entityPages.setup();
  _entityPages.setSize(230, 700);
  _entityPages.setShowHeader(false);

  _entityPage.setup();
  _entityPage.setName("Ent");
  _entityPage.add(&_entityPages);
  _pages.add(&_entityPage);

  _observersPage.setup("Obs");
  auto observersGroup = new ofxGuiGroupExtended(_appParams.observers);
  _observersPage.add(observersGroup);
  _observersPage.setSize(230, 700);
  observersGroup->setPosition(15, 100);
  _entityPages.add(&_observersPage);

  _occurrencesPage.setup("Occ");
  _occurrencesPage.add(new ofxGuiGroupExtended(_appParams.occurrences));
  _occurrencesPage.setSize(230, 700);
  _entityPages.add(&_occurrencesPage);

  _navigatorsPage.setup("Nav");
  _navigatorsPage.add(new ofxGuiGroupExtended(_appParams.navigators));
  _navigatorsPage.setSize(230, 700);
  _entityPages.add(&_navigatorsPage);

  _animationsPage.setup("Ani");
  _animationsPage.add(new ofxGuiGroupExtended(_appParams.animations));
  _pages.add(&_animationsPage);

  _colorsPage.setup("Col");
  _colorsPage.add(new ofxGuiGroupExtended(_appParams.colors));
  _pages.add(&_colorsPage);

  _renderingPage.setup("Ren");
  _renderingPage.add(new ofxGuiGroupExtended(_appParams.rendering));
  _pages.add(&_renderingPage);

  _physicsPage.setup("Phy");
  _physicsPage.add(new ofxGuiGroupExtended(_appParams.physics));
  _pages.add(&_physicsPage);

  _debugPage.setup("Dbg");
  _debugPage.add(new ofxGuiGroupExtended(_appParams.debug));
  _pages.add(&_debugPage);

  _actions.setPosition(0, 0);

  setBackgroundAlpha(&_pages, 0.5);
  setBackgroundAlpha(&_entityPages, 0.7);
}

AppGui::AppGui(MemoryAppParameters& appParams,
               AppActionHandler& actionHandler)
: _impl(new AppGuiImpl(appParams, actionHandler)) {}

void AppGui::draw() {
  _impl->draw();
}
