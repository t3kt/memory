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
#include "../core/Common.h"
#include "../core/Events.h"
#include "../navigation/NavigatorEntity.h"

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
};

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

extern EnumTypeInfo<SimulationEventType> SimulationEventTypeType;
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

class AnimationObject;
using AnimationEventArgs = SimulationEventArgs<AnimationObject>;
using AnimationEvent = SimulationEvent<AnimationObject>;

class OccurrenceEntity;
using OccurrenceEventArgs = SimulationEventArgs<OccurrenceEntity>;
using OccurrenceEvent = SimulationEvent<OccurrenceEntity>;

class ObserverEntity;
using ObserverEventArgs = SimulationEventArgs<ObserverEntity>;
using ObserverEvent = SimulationEvent<ObserverEntity>;

class NavigatorEntity;
using NavigatorEventArgs = SimulationEventArgs<NavigatorEntity>;
using NavigatorEvent = SimulationEvent<NavigatorEntity>;

class SimulationEvents {
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

  AbstractEvent* getEvent(SimulationEventType type);

  template<typename T>
  SimulationEvent<T>& spawned();

  template<typename T>
  SimulationEvent<T>& died();
};

#endif /* SimulationEvents_h */
