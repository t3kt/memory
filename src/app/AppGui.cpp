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
  void setupCorePage();
  void setupObserversPage();
  void setupOccurrencesPage();
  void setupAnimationsPage();
  void setupColorsPage();

  MemoryAppParameters& _appParams;

  ofxGuiPage _corePage;
  ofxGuiPage _observersPage;
  ofxGuiPage _occurrencesPage;
  ofxGuiPage _animationsPage;
  ofxGuiPage _colorsPage;

  ActionsGui _actions;

  ofxTabbedPages _pages;

  ofxFpsPlotter _fps;
};

AppGuiImpl::AppGuiImpl(MemoryAppParameters& appParams,
                       ofxLiquidEvent<void>& onLoad,
                       ofxLiquidEvent<void>& onSave)
: _appParams(appParams)
, _actions(onLoad, onSave) {
  _pages.setup();
  _pages.setSize(400, 700);
  _pages.setShowHeader(false);

  setupCorePage();
  _pages.add(&_corePage);

  setupObserversPage();
  _pages.add(&_observersPage);

  setupOccurrencesPage();
  _pages.add(&_occurrencesPage);

  setupAnimationsPage();
  _pages.add(&_animationsPage);

  setupColorsPage();
  _pages.add(&_colorsPage);

  _actions.setPosition(_pages.getWidth() + 10, 0);
}

void AppGuiImpl::setupCorePage() {
  _corePage.setup("Core");
  _corePage.add(new ofxGuiGroupExtended(_appParams.core));
}

void AppGuiImpl::setupObserversPage() {
  _observersPage.setup("Observers");
  _observersPage.add(new ofxGuiGroupExtended(_appParams.observers));
}

void AppGuiImpl::setupOccurrencesPage() {
  _occurrencesPage.setup("Occurrences");
  _occurrencesPage.add(new ofxGuiGroupExtended(_appParams.occurrences));
}

void AppGuiImpl::setupAnimationsPage() {
  _animationsPage.setup("Animations");
  _animationsPage.add(new ofxGuiGroupExtended(_appParams.animations));
}

void AppGuiImpl::setupColorsPage() {
  _colorsPage.setup("Colors");
  _colorsPage.add(new ofxGuiGroupExtended(_appParams.colors));
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
