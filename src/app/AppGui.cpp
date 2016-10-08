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
  rootTabs->setTabHeight(6);
  rootTabs->setTabWidth(44);
  auto coreTab = rootTabs->addGroup(_appParams.core);
  {
    auto entityTabs = rootTabs->addTabs("Ent");
    entityTabs->setTabHeight(6);
    entityTabs->addGroup(_appParams.observers)->setName("Obs");
    entityTabs->addGroup(_appParams.occurrences)->setName("Occ");
    entityTabs->addGroup(_appParams.navigators)->setName("Nav");
  }
  {
    auto visTabs = rootTabs->addTabs("Vis");
    visTabs->setTabHeight(6);
    visTabs->addGroup(_appParams.animations)->setName("Ani");
    visTabs->addGroup(_appParams.colors)->setName("Col");
    {
      auto renderTabs = visTabs->addTabs("Ren");
      renderTabs->setTabHeight(6);
      renderTabs->setTabWidth(44);
      renderTabs->addGroup(_appParams.rendering.camera)->setName("Cam");
      renderTabs->addGroup(_appParams.rendering.observers)->setName("Obs");
      renderTabs->addGroup(_appParams.rendering.occurrences)->setName("Occ");
      renderTabs->addGroup(_appParams.rendering.fog)->setName("Fog");
      renderTabs->addGroup(_appParams.rendering.postProc)->setName("Post");
    }
  }
  rootTabs->addGroup(_appParams.physics)->setName("Phys");
  rootTabs->addGroup(_appParams.debug)->setName("Dbg");
  {
    auto actionsTab = rootTabs->addGroup();
    actionsTab->setName("Act");
    addActionButtons(actionsTab);
  }
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
