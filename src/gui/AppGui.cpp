//
//  AppGui.cpp
//

#include "../gui/AppGui.h"

static void setBackgroundAlpha(ofxBaseGui* gui, float alpha) {
  gui->setBackgroundColor(ofColor(gui->getBackgroundColor(), alpha*255));
}

AppGui::AppGui(MemoryAppParameters& appParams)
: _appParams(appParams) { }

void AppGui::setup(AppActionHandler& actionHandler) {
  _pages = std::make_shared<ofxTabbedPages>();
  _pages->setup();
  _pages->setSize(240, 700);
  _pages->setShowHeader(false);
  _pages->setPosition(0, 60);

  _corePage = std::make_shared<ofxGuiPage>();
  _corePage->setup("Core");
  _corePage->add(new ofxGuiGroupExtended(_appParams.core));
  _pages->add(_corePage.get());

  _entityPages = std::make_shared<ofxTabbedPages>();
  _entityPages->setup();
  _entityPages->setSize(230, 700);
  _entityPages->setShowHeader(false);

  _entityPage = std::make_shared<ofxTabbedPages>();
  _entityPage->setup();
  _entityPage->setName("Ent");
  _entityPage->add(_entityPages.get());
  _pages->add(_entityPage.get());

  _observersPage = std::make_shared<ofxGuiPage>();
  _observersPage->setup("Obs");
  auto observersGroup = new ofxGuiGroupExtended(_appParams.observers);
  _observersPage->add(observersGroup);
  _observersPage->setSize(230, 700);
  observersGroup->setPosition(15, 100);
  _entityPages->add(_observersPage.get());

  _occurrencesPage = std::make_shared<ofxGuiPage>();
  _occurrencesPage->setup("Occ");
  _occurrencesPage->add(new ofxGuiGroupExtended(_appParams.occurrences));
  _occurrencesPage->setSize(230, 700);
  _entityPages->add(_occurrencesPage.get());

  _navigatorsPage = std::make_shared<ofxGuiPage>();
  _navigatorsPage->setup("Nav");
  _navigatorsPage->add(new ofxGuiGroupExtended(_appParams.navigators));
  _navigatorsPage->setSize(230, 700);
  _entityPages->add(_navigatorsPage.get());

  _animationsPage = std::make_shared<ofxGuiPage>();
  _animationsPage->setup("Anim");
  _animationsPage->add(new ofxGuiGroupExtended(_appParams.animations));
  _pages->add(_animationsPage.get());

  _colorsPage = std::make_shared<ofxGuiPage>();
  _colorsPage->setup("Col");
  _colorsPage->add(new ofxGuiGroupExtended(_appParams.colors));
  _pages->add(_colorsPage.get());

  _renderingPage = std::make_shared<ofxGuiPage>();
  _renderingPage->setup("Rend");
  _renderingPage->add(new ofxGuiGroupExtended(_appParams.rendering));
  _pages->add(_renderingPage.get());

  _physicsPage = std::make_shared<ofxGuiPage>();
  _physicsPage->setup("Phys");
  _physicsPage->add(new ofxGuiGroupExtended(_appParams.physics));
  _pages->add(_physicsPage.get());

  _actions = std::make_shared<ActionsGui>(actionHandler);
  _actions->setPosition(0, 0);

  setBackgroundAlpha(_pages.get(), 0.5);
  setBackgroundAlpha(_entityPages.get(), 0.7);
}

void AppGui::draw() {
  _pages->draw();
  _actions->draw();
}
