//
//  ControlApp.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include "ControlApp.h"
#include "SimulationApp.h"
#include <map>

static std::map<int, AppAction> KEY_TO_ACTION = {
  {'h', AppAction::RESET_CAMERA},
  {'l', AppAction::TOGGLE_LOGGING},
  {' ', AppAction::TOGGLE_CLOCK_STATE},
  {'9', AppAction::SPAWN_FEW_OCCURRENCES},
  {'0', AppAction::SPAWN_FEW_OBSERVERS},
  {'(', AppAction::SPAWN_MANY_OCCURRENCES},
  {')', AppAction::SPAWN_MANY_OBSERVERS},
  {'-', AppAction::KILL_FEW_OBSERVERS},
  {'_', AppAction::KILL_MANY_OBSERVERS},
  {'r', AppAction::LOAD_SETTINGS},
  {'w', AppAction::SAVE_SETTINGS},
  {'x', AppAction::STOP_ALL_ENTITIES},
  {'p', AppAction::TOGGLE_SHOW_PHYSICS},
  {'b', AppAction::TOGGLE_SHOW_BOUNDS},
};

template<typename A>
std::function<void(A&)> makeEntityEventLogger(const std::string message) {
  return [=](A& e) {
    ofLogNotice() << message << e.entity();
  };
}

std::function<void(AnimationEventArgs&)>
makeAnimationLogger(const std::string message) {
  return makeEntityEventLogger<AnimationEventArgs>(message);
}

std::function<void(ObserverEventArgs&)>
makeObserverLogger(const std::string message) {
  return makeEntityEventLogger<ObserverEventArgs>(message);
}

std::function<void(OccurrenceEventArgs&)>
makeOccurrenceLogger(const std::string message) {
  return makeEntityEventLogger<OccurrenceEventArgs>(message);
}

class EventLoggers {
public:
  EventLoggers()
  : _animationSpawned(makeAnimationLogger("Animation spawned: "))
  , _animationDied(makeAnimationLogger("Animation died: "))
  , _observerSpawned(makeObserverLogger("Observer spawned: "))
  , _observerDied(makeObserverLogger("Observer died: "))
  , _occurrenceSpawned(makeOccurrenceLogger("Occurrence spawned: "))
  , _occurrenceSpawnFailed(makeOccurrenceLogger("Occurrence spawn failed: "))
  , _occurrenceDied(makeOccurrenceLogger("Occurrence died: ")) {}

  void attach(SimulationEvents& events) {
    detach(events);
    events.animationSpawned.addListener(_animationSpawned, this);
    events.animationDied.addListener(_animationDied, this);
    events.observerSpawned.addListener(_observerSpawned, this);
    events.observerDied.addListener(_observerDied, this);
    events.occurrenceSpawned.addListener(_occurrenceSpawned, this);
    events.occurrenceSpawnFailed.addListener(_occurrenceSpawnFailed, this);
    events.occurrenceDied.addListener(_occurrenceDied, this);
  }
  void detach(SimulationEvents& events) {
    events.animationSpawned.removeListeners(this);
    events.animationDied.removeListeners(this);
    events.observerSpawned.removeListeners(this);
    events.observerDied.removeListeners(this);
    events.occurrenceSpawned.removeListeners(this);
    events.occurrenceSpawnFailed.removeListeners(this);
    events.occurrenceDied.removeListeners(this);
  }
private:
  std::function<void(AnimationEventArgs&)> _animationSpawned;
  std::function<void(AnimationEventArgs&)> _animationDied;
  std::function<void(ObserverEventArgs&)> _observerSpawned;
  std::function<void(ObserverEventArgs&)> _observerDied;
  std::function<void(OccurrenceEventArgs&)> _occurrenceSpawned;
  std::function<void(OccurrenceEventArgs&)> _occurrenceSpawnFailed;
  std::function<void(OccurrenceEventArgs&)> _occurrenceDied;
};

void ControlApp::attachSimulation(std::shared_ptr<SimulationApp> simulation) {
  _simulation = simulation;
}

void ControlApp::setup() {
  _eventLoggers = std::make_shared<EventLoggers>();

  _appParams.core.debug.loggingEnabledChanged += [this]() {
    updateLogState();
  };

  updateLogState();
  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams, *this);
}

void ControlApp::updateLogState() {
  bool enabled = _appParams.core.debug.loggingEnabled();
  ofSetLogLevel(enabled ? OF_LOG_NOTICE : OF_LOG_ERROR);
  if (enabled) {
    _eventLoggers->attach(_simulation->getEvents());
  } else {
    _eventLoggers->detach(_simulation->getEvents());
  }
}

void ControlApp::draw() {
  _gui->draw();
}

void ControlApp::keyPressed(int key) {
  auto iter = KEY_TO_ACTION.find(key);
  if (iter == KEY_TO_ACTION.end()) {
    return;
  }
  performAction(iter->second);
}

void ControlApp::performAction(AppAction action) {
  switch (action) {
    case AppAction::LOAD_SETTINGS:
      loadSettings();
      break;
    case AppAction::SAVE_SETTINGS:
      saveSettings();
      break;
    default:
      _simulation->performAction(action);
  }
}

void ControlApp::loadSettings() {
  ofLogNotice() << "Reading JSON settings...";
  _appParams.readFromFile("settings.json");
  ofLogNotice() << ".. read from JSON finished\n\t" << _appParams;
}

void ControlApp::saveSettings() {
  ofLogNotice() << "Writing JSON settings...";
  _appParams.writeToFile("settings.json");
  ofLogNotice() << ".. write to JSON finished";
}
