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
  _eventLoggers = std::make_shared<EventLoggers>();

  _appParams.core.debug.logging.enabled.changed += [this]() {
    updateLogState();
  };

  updateLogState();
  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams, *this);

  _midi = std::make_shared<MidiController>(_appParams);
  _midi->setup();

  registerAsActionHandler();
}

void ControlApp::updateLogState() {
  bool enabled = _appParams.core.debug.logging.enabled.get();
  ofSetLogLevel(enabled ? OF_LOG_NOTICE : OF_LOG_ERROR);
  auto simulation = AppSystem::get().simulation();
  if (enabled) {
    _eventLoggers->attach(simulation->getEvents());
  } else {
    _eventLoggers->detach(simulation->getEvents());
  }
}

void ControlApp::update() {
  _midi->update();
}

void ControlApp::draw() {
  _gui->draw();
}

bool ControlApp::performAction(AppAction action) {
  switch (action) {
    case AppAction::LOAD_SETTINGS:
      loadSettings();
      break;
    case AppAction::SAVE_SETTINGS:
      saveSettings();
      break;
    case AppAction::TOGGLE_LOGGING:
      _appParams.core.debug.logging.enabled.toggle();
      break;
    default:
      return false;
  }
  return true;
}

void ControlApp::loadSettings() {
  AppSystem::get().log().app().logNotice("Reading JSON settings...");
  _appParams.readFromFile("settings.json");
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << ".. read from JSON finished\n\t" << _appParams;
  });
}

void ControlApp::saveSettings() {
  AppSystem::get().log().app().logNotice("Writing JSON settings...");
  _appParams.writeToFile("settings.json");
  AppSystem::get().log().app().logNotice(".. write to JSON finished");
}
