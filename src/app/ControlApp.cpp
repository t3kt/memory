//
//  ControlApp.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include <map>
#include "../app/AppSystem.h"
#include "../app/ControlApp.h"
#include "../navigation/NavigatorEntity.h"
#include "../app/SimulationApp.h"
#include "../core/EventLogging.h"

void ControlApp::setup() {

  _gui = std::make_shared<AppGui>(_appParams, *this);

  _midi = std::make_shared<MidiController>(_appParams);
  _midi->setup();

  registerAsActionHandler();
}

void ControlApp::update() {
  _midi->update();
}

void ControlApp::draw() {
  _gui->draw();
}

bool ControlApp::performAction(AppAction action) {
  switch (action) {
    default:
      return false;
  }
  return true;
}
