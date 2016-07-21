//
//  ControlApp.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include <map>
#include "AppSystem.h"
#include "ControlApp.h"
#include "NavigatorEntity.h"
#include "SimulationApp.h"

template<typename A>
std::function<void(A&)> makeEntityEventLogger(const std::string message) {
  return [=](A& e) {
    ofLogNotice() << message << e.value();
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

std::function<void(NavigatorEventArgs&)>
makeNavigatorLogger(const std::string message) {
  return makeEntityEventLogger<NavigatorEventArgs>(message);
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
  , _occurrenceDied(makeOccurrenceLogger("Occurrence died: "))
  , _navigatorSpawned(makeNavigatorLogger("Navigator spawned: "))
  , _navigatorReachedLocation(makeNavigatorLogger("Navigator reached location: "))
  , _navigatorDied(makeNavigatorLogger("Navigator died: ")) {}

  void attach(SimulationEvents& events) {
    detach(events);
    events.animationSpawned.addListener(_animationSpawned, this);
    events.animationDied.addListener(_animationDied, this);
    events.observerSpawned.addListener(_observerSpawned, this);
    events.observerDied.addListener(_observerDied, this);
    events.occurrenceSpawned.addListener(_occurrenceSpawned, this);
    events.occurrenceSpawnFailed.addListener(_occurrenceSpawnFailed, this);
    events.occurrenceDied.addListener(_occurrenceDied, this);
    events.navigatorSpawned.addListener(_navigatorSpawned, this);
    events.navigatorReachedLocation.addListener(_navigatorReachedLocation, this);
    events.navigatorDied.addListener(_navigatorDied, this);
  }
  void detach(SimulationEvents& events) {
    events.animationSpawned.removeListeners(this);
    events.animationDied.removeListeners(this);
    events.observerSpawned.removeListeners(this);
    events.observerDied.removeListeners(this);
    events.occurrenceSpawned.removeListeners(this);
    events.occurrenceSpawnFailed.removeListeners(this);
    events.occurrenceDied.removeListeners(this);
    events.navigatorSpawned.removeListeners(this);
    events.navigatorReachedLocation.removeListeners(this);
    events.navigatorDied.removeListeners(this);
  }
private:
  std::function<void(AnimationEventArgs&)> _animationSpawned;
  std::function<void(AnimationEventArgs&)> _animationDied;
  std::function<void(ObserverEventArgs&)> _observerSpawned;
  std::function<void(ObserverEventArgs&)> _observerDied;
  std::function<void(OccurrenceEventArgs&)> _occurrenceSpawned;
  std::function<void(OccurrenceEventArgs&)> _occurrenceSpawnFailed;
  std::function<void(OccurrenceEventArgs&)> _occurrenceDied;
  std::function<void(NavigatorEventArgs&)> _navigatorSpawned;
  std::function<void(NavigatorEventArgs&)> _navigatorReachedLocation;
  std::function<void(NavigatorEventArgs&)> _navigatorDied;
};

void ControlApp::setup() {
  _eventLoggers = std::make_shared<EventLoggers>();

  _appParams.core.debug.loggingEnabled.changed += [this]() {
    updateLogState();
  };

  updateLogState();
  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams, *this);

  _midi = std::make_shared<ControlController>(_appParams);
  _midi->setup();

  registerAsActionHandler();
}

void ControlApp::updateLogState() {
  bool enabled = _appParams.core.debug.loggingEnabled();
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

void ControlApp::keyPressed(ofKeyEventArgs& event) {
  AppSystem::get().handleKeyPressed(event);
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
      _appParams.core.debug.loggingEnabled.toggle();
      break;
    default:
      return false;
  }
  return true;
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
