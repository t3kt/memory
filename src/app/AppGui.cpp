//
//  AppGui.cpp
//

#include <ofxGuiExtended.h>
#include "../app/AppGui.h"
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
#include "../app/SimulationApp.h"
#include "../control/Command.h"
#include "../control/CommandsController.h"
#include "../control/ParametersController.h"
#include "../control/ParamPresets.h"

class CommandButton
: public ofxGuiButton {
public:
  explicit CommandButton(CommandPtr command)
  : ofxGuiButton(command->label())
  , _command(command) {
    setType(ofxGuiToggleType::FULLSIZE);
    addListener(this, &CommandButton::onClick);
  }
  ~CommandButton() {
    removeListener(this, &CommandButton::onClick);
  }
private:
  void onClick() {
    _command->perform();
  }
  CommandPtr _command;
};

class LoadPresetButton
: public ofxGuiButton {
public:
  explicit LoadPresetButton(std::string name,
                            const ParamPreset& preset)
  : ofxGuiButton("load " + name)
  , _preset(preset) {
    setType(ofxGuiToggleType::FULLSIZE);
    addListener(this, &LoadPresetButton::onClick);
  }

  ~LoadPresetButton() {
    removeListener(this, &LoadPresetButton::onClick);
  }
private:
  void onClick() {
    //    AppSystem::get().simulation().parameters().loadPreset(_preset);
    AppSystem::get().simulation().parameters().transitionToPreset(_preset);
  }
  const ParamPreset& _preset;
};

static TParamBase* getElementParam(ofxGuiElement* element) {
  if (element == nullptr || !element->hasAttribute("tparam")) {
    return nullptr;
  }
  auto attr = element->getAttribute<TParamBase*>("tparam");
  return attr;
}

static void collapseDisabled(ofxGuiElement* element) {
  if (element == nullptr) {
    return;
  }
  auto params =
    dynamic_cast<ParamsWithEnabled*>(getElementParam(element));
  if (params && !params->enabled.get()) {
    auto group = dynamic_cast<ofxGuiGroup*>(element);
    group->minimize();
  }
  auto container = dynamic_cast<ofxGuiContainer*>(element);
  if (!container) {
    return;
  }
  for (auto child : container->getControls()) {
    collapseDisabled(child);
  }
}

void AppGui::collapseDisabled() {
  ::collapseDisabled(_mainPanel);
}

void AppGui::setup() {
  _gui = std::make_shared<ofxGui>();
  _gui->setupFlexBoxLayout();
  _mainPanel = _gui->addPanel();
  _mainPanel->blockLayout(true);
  _mainPanel->setBackgroundColor(ofFloatColor(1, 1, 1, 0.5));
  auto rootGroup = _mainPanel->addGroup();
  {
    _showAdvanced.set("Show Advanced", false);
    auto btn = rootGroup->add(_showAdvanced);
    btn->setType(ofxGuiToggleType::FULLSIZE);
    _showAdvanced.addListener(this,
                              &AppGui::onShowAdvancedChanged);
  }

  _rootTabs = rootGroup->addTabs();
  _appParams.core.addGuiTo(_rootTabs);
  {
    auto entityTabs = _rootTabs->addTabs("Ent");
    _appParams.observers.addGuiTo(entityTabs)->setName("Obs");
    _appParams.occurrences.addGuiTo(entityTabs)->setName("Occ");
    _appParams.navigators.addGuiTo(entityTabs)->setName("Nav");
    entityTabs->setTabHeight(6);
  }
  {
    auto visTabs = _rootTabs->addTabs("Vis");
    _appParams.animations.addGuiTo(visTabs)->setName("Ani");
    _appParams.colors.addGuiTo(visTabs)->setName("Col");
    {
      auto renderTabs = visTabs->addTabs("Ren");
      _appParams.rendering.camera.addGuiTo(renderTabs)->setName("Cam");
      _appParams.rendering.observers.addGuiTo(renderTabs)->setName("Obs");
      _appParams.rendering.occurrences.addGuiTo(renderTabs)->setName("Occ");
      _appParams.rendering.connections.addGuiTo(renderTabs)->setName("Con");
      _appParams.rendering.fog.addGuiTo(renderTabs)->setName("Fog");
      _appParams.rendering.postProc.addGuiTo(renderTabs)->setName("Post");
      renderTabs->setTabHeight(6);
      renderTabs->setTabWidth(44);
    }
    visTabs->setTabHeight(6);
  }
  {
    auto physTabs = _rootTabs->addTabs("Phys");
    _appParams.physics.bounds.addGuiTo(physTabs)->setName("Bound");
    {
      auto entityTabs = physTabs->addTabs("Entity");
      _appParams.physics.observers.addGuiTo(entityTabs)->setName("Obs");
      _appParams.physics.occurrences.addGuiTo(entityTabs)->setName("Occ");
      entityTabs->setTabHeight(6);
    }
    {
      auto connTabs = physTabs->addTabs("Conn");
      _appParams.physics.observerObserverAttraction.addGuiTo(connTabs)->setName("Obs/Obs");
      _appParams.physics.observerOccurrenceForce.addGuiTo(connTabs)->setName("Obs/Occ");
      connTabs->setTabHeight(6);
    }
    {
      auto anchorTabs = physTabs->addTabs("Anchor");
      _appParams.physics.observerAnchorPointAttraction.addGuiTo(anchorTabs)->setName("Obs");
      _appParams.physics.occurrenceAnchorPointAttraction.addGuiTo(anchorTabs)->setName("Occ");
      anchorTabs->setTabHeight(6);
    }
    {
      auto forceTabs = physTabs->addTabs("Frc");
      _appParams.physics.rebound.addGuiTo(forceTabs)->setName("Rebound");
      _appParams.physics.spatialNoiseForce.addGuiTo(forceTabs)->setName("Noise");
      _appParams.physics.vortexNodes.addGuiTo(forceTabs)->setName("Vortex");
      _appParams.physics.damping.addGuiTo(forceTabs)->setName("Damp");
      forceTabs->setTabHeight(6);
      forceTabs->setTabWidth(54);
    }
    physTabs->setTabHeight(6);
    physTabs->setTabWidth(54);
  }
  _appParams.debug.addGuiTo(_rootTabs)->setName("Dbg");
  {
    auto commandsTab = _rootTabs->addGroup("Cmd");
    addCommandButtons(commandsTab);
  }
  {
    auto presetsTab = _rootTabs->addGroup("Preset");
    _presetsContainer = presetsTab->addGroup();
    addPresetButtons(_presetsContainer);
  }
  _rootTabs->setTabHeight(6);
  _rootTabs->setTabWidth(44);
  collapseDisabled();
  loadAdvancedElements();
  setAdvancedHidden(!_showAdvanced.get());

  loadTheme();
  _mainPanel->blockLayout(false);

  AppSystem::get().commands()
  .registerCommand("reloadGuiTheme", "Reload GUI Theme", [&](const CommandArgs&) {
    loadTheme();
    return true;
  })
  .withButton(true);
}

void AppGui::updatePresetButtons() {
  _mainPanel->blockLayout(true);
  _presetsContainer->clear();
  addPresetButtons(_presetsContainer);
  _mainPanel->blockLayout(false);
}

void AppGui::addPresetButtons(ofxGuiContainer *container) {
  const auto& presets =
    AppSystem::get().simulation().parameters().presets();
  for (std::size_t i = 0; i < presets.size(); i++) {
    const auto& preset = *(presets[i]);
    auto name = preset.name();
    if (name.empty()) {
      name = "[" + ofToString(i) + "]";
    }
    _presetsContainer->add<LoadPresetButton>(name, preset);
  }
}

void AppGui::addCommandButtons(ofxGuiContainer *container) {
  auto& commands =
  AppSystem::get().simulation().commands().commands();
  for (auto command : commands) {
    if (command.second->supportsButton()) {
      container->add<CommandButton>(command.second);
    }
  }
}

static
void collectAdvancedElements(ofxGuiElement* element,
                             std::vector<ofxGuiElement*>& results) {
  if (element == nullptr) {
    return;
  }
  auto param = getElementParam(element);
  if (param && param->tags()[PTags::advanced]) {
    results.push_back(element);
  }
  auto container = dynamic_cast<ofxGuiContainer*>(element);
  if (container != nullptr) {
    for (auto child : container->getControls()) {
      collectAdvancedElements(child, results);
    }
  }
}

void AppGui::loadAdvancedElements() {
  _advancedElements.clear();
  collectAdvancedElements(_rootTabs, _advancedElements);
}

void AppGui::onShowAdvancedChanged(bool &visible) {
  setAdvancedHidden(!visible);
}

void AppGui::setAdvancedHidden(bool hidden) {
  for (auto element : _advancedElements) {
    element->setHidden(hidden);
  }
}

void AppGui::loadTheme() {
  _mainPanel->loadTheme("theme.json");
}

void AppGui::draw() {
}
