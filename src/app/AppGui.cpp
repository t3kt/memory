//
//  AppGui.cpp
//

#include <ofxGuiExtended.h>
#include "../app/AppGui.h"
#include "../app/AppSystem.h"

class ActionButton
: public ofxGuiButton {
public:
  explicit ActionButton(AppAction action)
  : ofxGuiButton(enumToString(action))
  , _action(action) {
    setType(ofxGuiToggleType::FULLSIZE);
    addListener(this, &ActionButton::onClick);
  }
  ~ActionButton() {
    removeListener(this, &ActionButton::onClick);
  }
private:
  void onClick() {
    AppSystem::get().performAction(_action);
  }

  const AppAction _action;
};

//static void collapseDisabled(ofxGuiElement* element) {
//  if (element == nullptr) {
//    return;
//  }
//  auto container = dynamic_cast<ofxGuiContainer*>(element);
//  if (container == nullptr) {
//    return;
//  }
//  auto __NAME = element->getName().c_str();
//  auto params = dynamic_cast<ParamsWithEnabled*>(&container->getParameter());
//  if (params != nullptr && !params->enabled.get()) {
//    auto group = dynamic_cast<ofxGuiGroup*>(container);
//    if (group != nullptr) {
//      group->minimize();
//    }
//  }
//  for (auto& child : container->children<ofxGuiContainer>()) {
//    collapseDisabled(child);
//  }
//
//}

void AppGui::collapseDisabled() {
//  ::collapseDisabled(_mainPanel);
}

void AppGui::setup() {
  _gui = std::make_shared<ofxGui>();
  _gui->setupFlexBoxLayout();
  _mainPanel = _gui->addPanel();
  _mainPanel->blockLayout(true);
  _mainPanel->setBackgroundColor(ofFloatColor(1, 1, 1, 0.5));
  auto rootGroup = _mainPanel->addGroup();

  auto rootTabs = rootGroup->addTabs();
  auto coreTab = rootTabs->addGroup(_appParams.core);
  {
    auto entityTabs = rootTabs->addTabs("Ent");
    entityTabs->addGroup(_appParams.observers)->setName("Obs");
    entityTabs->addGroup(_appParams.occurrences)->setName("Occ");
    entityTabs->addGroup(_appParams.navigators)->setName("Nav");
    entityTabs->setTabHeight(6);
  }
  {
    auto visTabs = rootTabs->addTabs("Vis");
    visTabs->addGroup(_appParams.animations)->setName("Ani");
    visTabs->addGroup(_appParams.colors)->setName("Col");
    {
      auto renderTabs = visTabs->addTabs("Ren");
      renderTabs->addGroup(_appParams.rendering.camera)->setName("Cam");
      renderTabs->addGroup(_appParams.rendering.observers)->setName("Obs");
      renderTabs->addGroup(_appParams.rendering.occurrences)->setName("Occ");
      renderTabs->addGroup(_appParams.rendering.connections)->setName("Con");
      renderTabs->addGroup(_appParams.rendering.fog)->setName("Fog");
      renderTabs->addGroup(_appParams.rendering.postProc)->setName("Post");
      renderTabs->setTabHeight(6);
      renderTabs->setTabWidth(44);
    }
    visTabs->setTabHeight(6);
  }
  {
    auto physTabs = rootTabs->addTabs("Phys");
    physTabs->addGroup(_appParams.physics.bounds)->setName("Bound");
    {
      auto entityTabs = physTabs->addTabs("Entity");
      entityTabs->addGroup(_appParams.physics.observers)->setName("Obs");
      entityTabs->addGroup(_appParams.physics.occurrences)->setName("Occ");
      entityTabs->setTabHeight(6);
    }
    {
      auto connTabs = physTabs->addTabs("Conn");
      connTabs->addGroup(_appParams.physics.observerObserverAttraction)->setName("Obs/Obs");
      connTabs->addGroup(_appParams.physics.observerOccurrenceForce)->setName("Obs/Occ");
      connTabs->setTabHeight(6);
    }
    {
      auto anchorTabs = physTabs->addTabs("Anchor");
      anchorTabs->addGroup(_appParams.physics.observerAnchorPointAttraction)->setName("Obs");
      anchorTabs->addGroup(_appParams.physics.occurrenceAnchorPointAttraction)->setName("Occ");
      anchorTabs->setTabHeight(6);
    }
    {
      auto forceTabs = physTabs->addTabs("Frc");
      forceTabs->addGroup(_appParams.physics.rebound)->setName("Rebound");
      forceTabs->addGroup(_appParams.physics.spatialNoiseForce)->setName("Noise");
      forceTabs->addGroup(_appParams.physics.vortexNodes)->setName("Vortex");
      forceTabs->addGroup(_appParams.physics.damping)->setName("Damp");
      forceTabs->setTabHeight(6);
      forceTabs->setTabWidth(54);
    }
    physTabs->setTabHeight(6);
    physTabs->setTabWidth(54);
  }
  rootTabs->addGroup(_appParams.debug)->setName("Dbg");
  {
    auto actionsTab = rootTabs->addGroup();
    actionsTab->setName("Act");
    addActionButtons(actionsTab);
  }
  rootTabs->setTabHeight(6);
  rootTabs->setTabWidth(44);
  collapseDisabled();

  loadTheme();
  _mainPanel->blockLayout(false);
}

void AppGui::addActionButtons(ofxGuiContainer *container) {
  for (auto action : enumValues<AppAction>()) {
    if (action == AppAction::NONE) {
      continue;
    }
    container->add<ActionButton>(action);
  }
}

void AppGui::loadTheme() {
  _mainPanel->loadTheme("theme.json");
}

bool AppGui::performAction(AppAction action) {
  switch (action) {
    case AppAction::RELOAD_THEME:
      loadTheme();
      return true;
    default:
      return false;
  }
}

void AppGui::draw() {
}
