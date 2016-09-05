//
//  SimulationApp.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include <ofSystemUtils.h>
#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../core/EventLogging.h"

void SimulationApp::setup() {
  loadSettings();

  _eventLoggers = std::make_shared<EventLoggers>();

  _appParams.debug.logging.enabled.changed += [this]() {
    updateLogState();
  };

  updateLogState();

  _renderingController =
  std::make_shared<RenderingController>(_appParams.rendering,
                                        getWindow(),
                                        _context);
  _renderingController->setup();

  _appParams.core.output.fullscreen.changed += [&](bool& fullscreen) {
    _window->setFullscreen(fullscreen);
    _renderingController->updateResolution();
  };

  registerAsActionHandler();

  _observers =
  std::make_shared<ObserversController>(_appParams.observers,
                                        _appParams.core.bounds,
                                        _context,
                                        _events);
  _observers->setup();

  _occurrences =
  std::make_shared<OccurrencesController>(_appParams.occurrences,
                                          _appParams.core.bounds,
                                          *_observers,
                                          _context,
                                          _events);
  _occurrences->setup();

  _animations =
  std::make_shared<AnimationsController>(_appParams.animations,
                                         _events,
                                         _context);
  _animations->setup();

  _physics = std::make_shared<PhysicsController>(_appParams.physics,
                                                 _appParams.core.bounds,
                                                 _appParams.debug,
                                                 _context);
  _physics->setup();

  _clock = std::make_shared<Clock>(_appParams.core.clock, _context.state);
  _clock->setup();

  _statusController = std::make_shared<StatusInfoController>(_context);

  _inspectionController =
  std::make_shared<InspectionController>(_appParams.debug.inspect,
                                         _context,
                                         *_window);
  _inspectionController->setup();

  _navigators =
  std::make_shared<NavigatorsController>(_context,
                                         _appParams.navigators,
                                         _events);
  _navigators->setup();

  _gui = std::make_shared<AppGui>(_appParams, *this);

  _midi = std::make_shared<MidiController>(_appParams);
  _midi->setup();

  _osc = std::make_shared<OscController>(_appParams);

#ifdef ENABLE_SYPHON
  _syphonServer.setName("Memory Main Output");
#endif
}

void SimulationApp::updateLogState() {
  bool enabled = _appParams.debug.logging.enabled.get();
  ofSetLogLevel(enabled ? OF_LOG_NOTICE : OF_LOG_ERROR);
  auto simulation = AppSystem::get().simulation();
  if (enabled) {
    _eventLoggers->attach(simulation->getEvents());
  } else {
    _eventLoggers->detach(simulation->getEvents());
  }
}

void SimulationApp::update() {
  _osc->update();
  _midi->update();
  _clock->update();
  _observers->update();
  _occurrences->update();
  _animations->update();
  _physics->update();
  _navigators->update();
  _renderingController->update();

  _context.highlightedEntities.clear();
  _inspectionController->update();

  if (_appParams.debug.showStatus()) {
    _statusController->update();
  }
}

void SimulationApp::draw() {
  _renderingController->beginDraw();

  _renderingController->draw();
  _observers->draw();
  _occurrences->draw();
  _animations->draw();
  _navigators->draw();
  _physics->draw();

  if (_appParams.debug.showBounds()) {
    ofPushStyle();
    ofNoFill();
    ofSetColor(_appParams.colors.bounds.get());
    ofDrawBox(_appParams.core.bounds.size());
    ofPopStyle();
    ofDrawGrid(
               _appParams.core.bounds.size.get() / 2 / 4, // step size
               4, // number of steps
               true // labels
               );
  }

  _renderingController->endDraw();

#ifdef ENABLE_SYPHON
  if (_appParams.core.output.externalEnabled()) {
    _renderingController->pushToSyphon(_syphonServer);
  }
#endif

  if (_appParams.debug.showStatus()) {
    _statusController->draw();
  }

  _inspectionController->draw();
  _gui->draw();
}

void SimulationApp::dumpEntityState() {
  Json state = _context.to_json();
  JsonUtil::prettyPrintJsonToStream(state, std::cout);
}

void SimulationApp::loadEntityState() {
  FileAction action = [&](ofFileDialogResult& file) {
    _context.readFromFile(file.getPath());
    return true;
  };
  AppSystem::get().performFileLoadAction(action,
                                         "Load Entity State",
                                         "entityState.json");
}

void SimulationApp::saveEntityState() {
  FileAction action = [&](ofFileDialogResult& file) {
    _context.writeToFile(file.getPath());
    return true;
  };
  AppSystem::get().performFileSaveAction(action,
                                         "Save Entity State",
                                         "entityState.json");
}

void SimulationApp::keyPressed(ofKeyEventArgs& event) {
  AppSystem::get().handleKeyPressed(event);
}

bool SimulationApp::performAction(AppAction action) {
  switch (action) {
    case AppAction::DUMP_ENTITY_STATE:
      dumpEntityState();
      break;
    case AppAction::LOAD_ENTITY_STATE:
      loadEntityState();
      break;
    case AppAction::SAVE_ENTITY_STATE:
      saveEntityState();
      break;
    case AppAction::SPAWN_LOAD_TEST_ENTITIES:
      _observers->performAction(AppAction::SPAWN_TONS_OF_OBSERVERS);
      _occurrences->performAction(AppAction::SPAWN_TONS_OF_OCCURRENCES);
      break;
    case AppAction::LOAD_SETTINGS:
      loadSettings();
      break;
    case AppAction::SAVE_SETTINGS:
      saveSettings();
      break;
    case AppAction::TOGGLE_LOGGING:
      _appParams.debug.logging.enabled.toggle();
      break;
    default:
      return false;
  }
  return true;
}

void SimulationApp::loadSettings() {
  AppSystem::get().log().app().logNotice("Reading JSON settings...");
  _appParams.readFromFile("settings.json");
  AppSystem::get().log().app().logNotice([&](ofLog& log) {
    log << ".. read from JSON finished\n\t" << _appParams;
  });
}

void SimulationApp::saveSettings() {
  AppSystem::get().log().app().logNotice("Writing JSON settings...");
  _appParams.writeToFile("settings.json");
  AppSystem::get().log().app().logNotice(".. write to JSON finished");
}
