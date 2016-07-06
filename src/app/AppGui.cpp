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
: public ofxGuiGroupExtended {
public:
  ActionsGui(ofxLiquidEvent<void>& onLoad, ofxLiquidEvent<void>& onSave) {
    setup();
    setName("Actions");

    auto loadButton = new ofxMinimalButton("load");
    loadButton->addListener(&onLoad, &ofxLiquidEvent<void>::notifyListeners);
    add(loadButton);

    auto saveButton = new ofxMinimalButton("save");
    saveButton->addListener(&onSave, &ofxLiquidEvent<void>::notifyListeners);
    add(saveButton);
  }
};

class AppGuiImpl {
public:
  AppGuiImpl(MemoryAppParameters& appParams,
             ofxLiquidEvent<void>& onLoad,
             ofxLiquidEvent<void>& onSave);

  void update() {
    _fps.update();
  }
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

  ofxTabbedPages _pages;

  ofxGuiPage _observersPage;
  ofxGuiPage _occurrencesPage;

  ofxTabbedPages _entityPages;

  ActionsGui _actions;
  ofxFpsPlotter _fps;
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
  _pages.setSize(250, 700);
  _pages.setShowHeader(false);

  _corePage.setup("Core");
  _corePage.add(new ofxGuiGroupExtended(_appParams.core));
  _pages.add(&_corePage);

  _entityPages.setup();
  _entityPages.setSize(240, 700);
  _entityPages.setShowHeader(false);

  _entityPage.setup();
  _entityPage.setName("Ent");
  _entityPage.add(&_entityPages);
  _pages.add(&_entityPage);

  _observersPage.setup("Observers");
  _observersPage.add(new ofxGuiGroupExtended(_appParams.observers));
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

  _actions.setPosition(_pages.getWidth() + 10, 0);

  setBackgroundAlpha(&_pages, 0.5);
  setBackgroundAlpha(&_entityPages, 0.7);
}

AppGui::AppGui(MemoryAppParameters& appParams)
: _appParams(appParams) { }

void AppGui::setup() {
  _impl = std::make_shared<AppGuiImpl>(_appParams, onLoad, onSave);
}

void AppGui::update() {
  _impl->update();
}

void AppGui::draw() {
  _impl->draw();
}
