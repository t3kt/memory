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

  _gui = _components.add<AppGui>(_appParams);

  _actions =
  _components.add<ActionsController>(_context);

  _midi = _components.add<MidiController>(_appParams);

  _osc = _components.add<OscController>(_appParams);

  _clock =
  _components.add<Clock>(_appParams.core.clock,
                         _context.state);

  _physics =
  _components.add<PhysicsController>(_appParams.physics,
                                     _appParams.debug,
                                     _context);

  _nodes =
  _components.add<NodesController>(_context,
                                   _events);

  _observers =
  _components.add<ObserversController>(_appParams.observers,
                                       _physics->bounds(),
                                       _context,
                                       _events);

  _occurrences =
  _components.add<OccurrencesController>(_appParams.occurrences,
                                         _physics->bounds(),
                                         *_observers,
                                         _context,
                                         _events);

  _animations =
  _components.add<AnimationsController>(_appParams.animations,
                                        _events,
                                        _context);

  _navigators =
  _components.add<NavigatorsController>(_context,
                                        _appParams.navigators,
                                        _events);

  _renderingController =
  _components.add<RenderingController>(_appParams.rendering,
                                        *_window,
                                        _context);

  _appParams.core.output.fullscreen.changed += [&](bool& fullscreen) {
    _window->setFullscreen(fullscreen);
    _renderingController->updateResolution();
  };

  _inspectionController =
  _components.add<InspectionController>(_appParams.debug.inspect,
                                        _context,
                                        *_window);

  _statusController =
  _components.add<StatusInfoController>(_appParams.debug,
                                        _context);

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
  _components.update();
}

void SimulationApp::draw() {
  _renderingController->beginDraw();

  _renderingController->draw();
  _animations->draw();
  _navigators->draw();
  _physics->draw();

  _renderingController->endDraw();

#ifdef ENABLE_SYPHON
  if (_appParams.core.output.externalEnabled()) {
    _renderingController->pushToSyphon(_syphonServer);
  }
#endif

  _statusController->draw();
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
      AppSystem::get().performAction(AppAction::SPAWN_TONS_OF_OBSERVERS);
      AppSystem::get().performAction(AppAction::SPAWN_TONS_OF_OCCURRENCES);
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
  if (_gui) {
    _gui->collapseDisabled();
  }
}

void SimulationApp::saveSettings() {
  AppSystem::get().log().app().logNotice("Writing JSON settings...");
  _appParams.writeToFile("settings.json");
  AppSystem::get().log().app().logNotice(".. write to JSON finished");
}
