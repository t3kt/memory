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
#include "Common.h"
#include "Events.h"

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

class AnimationObject;
using AnimationEventArgs = SimulationEventArgs<AnimationObject>;
using AnimationEvent = TEvent<AnimationEventArgs>;

class OccurrenceEntity;
using OccurrenceEventArgs = SimulationEventArgs<OccurrenceEntity>;
using OccurrenceEvent = TEvent<OccurrenceEventArgs>;

class ObserverEntity;
using ObserverEventArgs = SimulationEventArgs<ObserverEntity>;
using ObserverEvent = TEvent<ObserverEventArgs>;

class NavigatorEntity;
using NavigatorEventArgs = SimulationEventArgs<NavigatorEntity>;
using NavigatorEvent = TEvent<NavigatorEventArgs>;

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
};

#endif /* SimulationEvents_h */
