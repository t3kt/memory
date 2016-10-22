//
//  SimulationEvents.h
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#ifndef SimulationEvents_h
#define SimulationEvents_h

#include <iostream>
#include <ofxTEnums.h>
#include "../core/Common.h"
#include "../core/Events.h"

enum class SimulationEventType {
  ANIMATION_SPAWNED,
  ANIMATION_DIED,
  OBSERVER_SPAWNED,
  OBSERVER_DIED,
  OCCURRENCE_SPAWNED,
  OCCURRENCE_SPAWN_FAILED,
  OCCURRENCE_DIED,
  NAVIGATOR_SPAWNED,
  NAVIGATOR_REACHED_LOCATION,
  NAVIGATOR_DIED,
  NODE_SPAWNED,
  NODE_DIED,
};

class AnimationObject;
class ObserverEntity;
class OccurrenceEntity;
class NavigatorEntity;
class NodeEntity;

template<typename T>
constexpr SimulationEventType spawnedEventType();

template<>
constexpr SimulationEventType spawnedEventType<AnimationObject>() {
  return SimulationEventType::ANIMATION_SPAWNED;
}

template<>
constexpr SimulationEventType spawnedEventType<ObserverEntity>() {
  return SimulationEventType::OBSERVER_SPAWNED;
}

template<>
constexpr SimulationEventType spawnedEventType<OccurrenceEntity>() {
  return SimulationEventType::OCCURRENCE_SPAWNED;
}

template<>
constexpr SimulationEventType spawnedEventType<NavigatorEntity>() {
  return SimulationEventType::NAVIGATOR_SPAWNED;
}

template<>
constexpr SimulationEventType spawnedEventType<NodeEntity>() {
  return SimulationEventType::NODE_SPAWNED;
}

template<typename T>
constexpr SimulationEventType diedEventType();

template<>
constexpr SimulationEventType diedEventType<AnimationObject>() {
  return SimulationEventType::ANIMATION_DIED;
}

template<>
constexpr SimulationEventType diedEventType<ObserverEntity>() {
  return SimulationEventType::OBSERVER_DIED;
}

template<>
constexpr SimulationEventType diedEventType<OccurrenceEntity>() {
  return SimulationEventType::OCCURRENCE_DIED;
}

template<>
constexpr SimulationEventType diedEventType<NavigatorEntity>() {
  return SimulationEventType::NAVIGATOR_DIED;
}

template<>
constexpr SimulationEventType diedEventType<NodeEntity>() {
  return SimulationEventType::NODE_DIED;
}

std::ostream& operator<<(std::ostream& os,
                         const SimulationEventType& value);

template<typename T>
class SimulationEventArgs
: public EventArgs {
public:
  SimulationEventArgs(SimulationEventType type, T& value)
  : _type(type)
  , _value(value) { }

  SimulationEventType type() const { return _type; }

  T& value() { return _value; }

  std::string typeName() const override { return "SimulationEventArgs"; }
private:
  void outputFields(std::ostream& os) const override {
    os << "type: " << _type;
    os << ", value: " << _value;
  }

  SimulationEventType _type;
  T& _value;
};

template<typename T>
using SimulationEvent = TEvent<SimulationEventArgs<T>>;

using AnimationEventArgs = SimulationEventArgs<AnimationObject>;
using AnimationEvent = SimulationEvent<AnimationObject>;

using OccurrenceEventArgs = SimulationEventArgs<OccurrenceEntity>;
using OccurrenceEvent = SimulationEvent<OccurrenceEntity>;

using ObserverEventArgs = SimulationEventArgs<ObserverEntity>;
using ObserverEvent = SimulationEvent<ObserverEntity>;

using NavigatorEventArgs = SimulationEventArgs<NavigatorEntity>;
using NavigatorEvent = SimulationEvent<NavigatorEntity>;

using NodeEventArgs = SimulationEventArgs<NodeEntity>;
using NodeEvent = SimulationEvent<NodeEntity>;

class SimulationEvents
: public NonCopyable {
public:
  AnimationEvent animationSpawned;
  AnimationEvent animationDied;

  ObserverEvent observerSpawned;
  ObserverEvent observerDied;

  OccurrenceEvent occurrenceSpawned;
  OccurrenceEvent occurrenceSpawnFailed;
  OccurrenceEvent occurrenceDied;

  NavigatorEvent navigatorSpawned;
  NavigatorEvent navigatorReachedLocation;
  NavigatorEvent navigatorDied;

  NodeEvent nodeSpawned;
  NodeEvent nodeDied;

  AbstractEvent* getEvent(SimulationEventType type);

  template<typename T>
  SimulationEvent<T>& spawned();

  template<typename T>
  void spawned(T& entity) {
    SimulationEventArgs<T> e(spawnedEventType<T>(), entity);
    spawned<T>().notifyListeners(e);
  }

  template<typename T>
  SimulationEvent<T>& died();

  template<typename T>
  void died(T& entity) {
    SimulationEventArgs<T> e(diedEventType<T>(), entity);
    died<T>().notifyListeners(e);
  }
};

#endif /* SimulationEvents_h */
