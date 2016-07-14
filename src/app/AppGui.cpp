//
//  AppGui.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "AppGui.h"
#include <ofxGuiExtended.h>

class ActionsGui
: public ofxPanelExtended {
public:
  ActionsGui(ofxLiquidEvent<void>& onLoad, ofxLiquidEvent<void>& onSave) {
    setup();
    setName("Actions");
    _group.setup();
    add(&_group);

    auto loadButton = new ofxMinimalButton("load");
    loadButton->addListener(&onLoad, &ofxLiquidEvent<void>::notifyListeners);
    _group.add(loadButton);

    auto saveButton = new ofxMinimalButton("save");
    saveButton->addListener(&onSave, &ofxLiquidEvent<void>::notifyListeners);
    _group.add(saveButton);

    _group.minimize();
  }
private:
  ofxGuiGroupExtended _group;
};

class AppGuiImpl {
public:
  AppGuiImpl(MemoryAppParameters& appParams,
             ofxLiquidEvent<void>& onLoad,
             ofxLiquidEvent<void>& onSave);

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

  ofxTabbedPages _pages;

  ofxGuiPage _observersPage;
  ofxGuiPage _occurrencesPage;

  ofxTabbedPages _entityPages;

  ActionsGui _actions;
};

static void setBackgroundAlpha(ofxBaseGui* gui, float alpha) {
  gui->setBackgroundColor(ofColor(gui->getBackgroundColor(), alpha*255));
}

AppGuiImpl::AppGuiImpl(MemoryAppParameters& appParams,
                       ofxLiquidEvent<void>& onLoad,
                       ofxLiquidEvent<void>& onSave)
: _appParams(appParams)
, _actions(onLoad, onSave) {
  _pages.setup();
  _pages.setSize(290, 600);
  _pages.setShowHeader(false);
  _pages.setPosition(0, 60);

  _corePage.setup("Core");
  _corePage.add(new ofxGuiGroupExtended(_appParams.core));
  _pages.add(&_corePage);

  _entityPages.setup();
  _entityPages.setSize(270, 700);
  _entityPages.setShowHeader(false);

  _entityPage.setup();
  _entityPage.setName("Ent");
  _entityPage.add(&_entityPages);
  _pages.add(&_entityPage);

  _observersPage.setup("Observers");
  auto observersGroup = new ofxGuiGroupExtended(_appParams.observers);
  _observersPage.add(observersGroup);
  observersGroup->setPosition(15, 100);
  _entityPages.add(&_observersPage);

  _occurrencesPage.setup("Occurrences");
  _occurrencesPage.add(new ofxGuiGroupExtended(_appParams.occurrences));
  _entityPages.add(&_occurrencesPage);

  _animationsPage.setup("Anim");
  _animationsPage.add(new ofxGuiGroupExtended(_appParams.animations));
  _pages.add(&_animationsPage);

  _colorsPage.setup("Col");
  _colorsPage.add(new ofxGuiGroupExtended(_appParams.colors));
  _pages.add(&_colorsPage);

  _renderingPage.setup("Rend");
  _renderingPage.add(new ofxGuiGroupExtended(_appParams.rendering));
  _pages.add(&_renderingPage);

  _physicsPage.setup("Phys");
  _physicsPage.add(new ofxGuiGroupExtended(_appParams.physics));
  _pages.add(&_physicsPage);

  _actions.setPosition(0, 0);

  setBackgroundAlpha(&_pages, 0.5);
  setBackgroundAlpha(&_entityPages, 0.7);
}

AppGui::AppGui(MemoryAppParameters& appParams)
: _appParams(appParams) { }

void AppGui::setup() {
  _impl = std::make_shared<AppGuiImpl>(_appParams, onLoad, onSave);
}

void AppGui::draw() {
  _impl->draw();
}
