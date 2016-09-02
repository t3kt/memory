//
//  AppGui.cpp
//

#include "../gui/AppGui.h"
#include "../gui/GuiControls.h"

static void setBackgroundAlpha(ofxBaseGui* gui, float alpha) {
  gui->setBackgroundColor(ofColor(gui->getBackgroundColor(), alpha*255));
}

AppGui::AppGui(MemoryAppParameters& appParams)
: _appParams(appParams) { }

void AppGui::setup(AppActionHandler& actionHandler) {
  _pages = new ofxTabbedPages();
  _pages->setup();
  _pages->setSize(240, 700);
  _pages->setShowHeader(false);
  _pages->setPosition(0, 60);

  _corePage = new ofxGuiPage();
  _corePage->setup("Core");
  _corePage->add(new ofxGuiGroupExtended(_appParams.core));
  _pages->add(_corePage);

  _entityPages = new ofxTabbedPages();
  _entityPages->setup();
  _entityPages->setSize(230, 700);
  _entityPages->setShowHeader(false);

  _entityPage = new ofxTabbedPages();
  _entityPage->setup();
  _entityPage->setName("Ent");
  _entityPage->add(_entityPages);
  _pages->add(_entityPage);

  _observersPage = new ofxGuiPage();
  _observersPage->setup("Obs");
  auto observersGroup = new ofxGuiGroupExtended(_appParams.observers);
  _observersPage->add(observersGroup);
  _observersPage->setSize(230, 700);
  observersGroup->setPosition(15, 100);
  _entityPages->add(_observersPage);

  _occurrencesPage = new ofxGuiPage();
  _occurrencesPage->setup("Occ");
  _occurrencesPage->add(new ofxGuiGroupExtended(_appParams.occurrences));
  _occurrencesPage->setSize(230, 700);
  _entityPages->add(_occurrencesPage);

  _navigatorsPage = new ofxGuiPage();
  _navigatorsPage->setup("Nav");
  _navigatorsPage->add(new ofxGuiGroupExtended(_appParams.navigators));
  _navigatorsPage->setSize(230, 700);
  _entityPages->add(_navigatorsPage);

  _animationsPage = new ofxGuiPage();
  _animationsPage->setup("Ani");
  {
    auto builder = TGuiBuilder(GuiGroupMode::GROUP);
    auto group = builder.createControl(_appParams.animations);
    _animationsPage->add(group);
  }
  _pages->add(_animationsPage);

  _colorsPage = new ofxGuiPage();
  _colorsPage->setup("Col");
  _colorsPage->add(new ofxGuiGroupExtended(_appParams.colors));
  _pages->add(_colorsPage);

  _renderingPage = new ofxGuiPage();
  _renderingPage->setup("Ren");
  {
    auto builder = TGuiBuilder(GuiGroupMode::GROUP);
    auto group = builder.createControl(_appParams.rendering);
    _renderingPage->add(group);
  }
  _pages->add(_renderingPage);

  _physicsPage = new ofxGuiPage();
  _physicsPage->setup("Phy");
  {
    auto builder = TGuiBuilder(GuiGroupMode::GROUP);
    auto group = builder.createControl(_appParams.physics);
    _physicsPage->add(group);
  }
  _pages->add(_physicsPage);

  _debugPage = new ofxGuiPage();
  _debugPage->setup("Dbg");
  _debugPage->add(new ofxGuiGroupExtended(_appParams.debug));
  _pages->add(_debugPage);

  _actions = new ActionsGui(actionHandler);
  _actions->setPosition(0, 0);

  setBackgroundAlpha(_pages, 0.5);
  setBackgroundAlpha(_entityPages, 0.7);
}

void AppGui::draw() {
  _pages->draw();
  _actions->draw();
}
