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

template<typename A>
std::function<void(A&)> makeEntityEventLogger(Logger& logger,
                                              const std::string message) {
  return [=](A& e) {
    logger.logNotice([&](ofLog& log) {
      log << message << e.value();
    });
  };
}

std::function<void(AnimationEventArgs&)>
makeAnimationLogger(const std::string message) {
  auto& logger = AppSystem::get().log().animation();
  return makeEntityEventLogger<AnimationEventArgs>(logger, message);
}

std::function<void(ObserverEventArgs&)>
makeObserverLogger(const std::string message) {
  auto& logger = AppSystem::get().log().observer();
  return makeEntityEventLogger<ObserverEventArgs>(logger, message);
}

std::function<void(OccurrenceEventArgs&)>
makeOccurrenceLogger(const std::string message) {
  auto& logger = AppSystem::get().log().occurrence();
  return makeEntityEventLogger<OccurrenceEventArgs>(logger, message);
}

std::function<void(NavigatorEventArgs&)>
makeNavigatorLogger(const std::string message) {
  auto& logger = AppSystem::get().log().navigation();
  return makeEntityEventLogger<NavigatorEventArgs>(logger, message);
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

  _appParams.core.debug.logging.enabled.changed += [this]() {
    updateLogState();
  };

  updateLogState();
  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams);
  _gui->setup(*this);

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
