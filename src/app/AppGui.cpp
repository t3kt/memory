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

    auto group = new ofxGuiGroupExtended();
    add(group);

    auto loadButton = new ofxMinimalButton("load");
    loadButton->addListener(&onLoad, &ofxLiquidEvent<void>::notifyListeners);
    group->add(loadButton);

    auto saveButton = new ofxMinimalButton("save");
    saveButton->addListener(&onSave, &ofxLiquidEvent<void>::notifyListeners);
    group->add(saveButton);
  }
};

AppGui::AppGui(MemoryAppParameters& appParams)
: _appParams(appParams) { }

void AppGui::setup() {
  _actions = std::make_shared<ActionsGui>(onLoad, onSave);
  _actions->setPosition(0, 0);
  _settings = std::make_shared<ofxPanelExtended>(_appParams);
  _settings->setPosition(0, _actions->getHeight() + 5);
  _settings->setName("Settings");
}

void AppGui::update() {
}

void AppGui::draw() {
  _actions->draw();
  _settings->draw();
}
