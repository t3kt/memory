//
//  EventLogging.cpp
//  memory
//
//  Created by tekt on 8/27/16.
//
//

#include "../app/AppSystem.h"
#include "../core/EventLogging.h"
#include "../core/Logging.h"
#include "../core/NodeEntity.h"
#include "../navigation/NavigatorEntity.h"

std::ostream& operator<<(std::ostream& os, const NodeEntity& e) {
  e.output(os);
  return os;
}

template<typename A>
std::function<void(A&)> makeEntityEventLogger(Logger& logger,
                                              const std::string message) {
  return [&](A& e) {
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

std::function<void(NodeEventArgs&)>
makeNodeLogger(const std::string message) {
  auto& logger = AppSystem::get().log().node();
  return makeEntityEventLogger<NodeEventArgs>(logger, message);
}

EventLoggers::EventLoggers()
: _animationSpawned(makeAnimationLogger("Animation spawned: "))
, _animationDied(makeAnimationLogger("Animation died: "))
, _observerSpawned(makeObserverLogger("Observer spawned: "))
, _observerDied(makeObserverLogger("Observer died: "))
, _occurrenceSpawned(makeOccurrenceLogger("Occurrence spawned: "))
, _occurrenceSpawnFailed(makeOccurrenceLogger("Occurrence spawn failed: "))
, _occurrenceDied(makeOccurrenceLogger("Occurrence died: "))
, _navigatorSpawned(makeNavigatorLogger("Navigator spawned: "))
, _navigatorReachedLocation(makeNavigatorLogger("Navigator reached location: "))
, _navigatorDied(makeNavigatorLogger("Navigator died: "))
, _nodeSpawned(makeNodeLogger("Node spawned: "))
, _nodeDied(makeNodeLogger("Node died: ")) {}

void EventLoggers::attach(SimulationEvents& events) {
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
  events.nodeSpawned.addListener(_nodeSpawned, this);
  events.nodeDied.addListener(_nodeDied, this);
}

void EventLoggers::detach(SimulationEvents& events) {
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
  events.nodeSpawned.removeListeners(this);
  events.nodeDied.removeListeners(this);
}
