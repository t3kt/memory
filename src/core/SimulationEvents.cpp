//
//  SimulationEvents.cpp
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#include "../app/AppSystem.h"
#include "../core/JsonIO.h"
#include "../core/SimulationEvents.h"

EnumTypeInfo<SimulationEventType> SimulationEventTypeInfo {
  {"animationSpawned", SimulationEventType::ANIMATION_SPAWNED},
  {"animationDied", SimulationEventType::ANIMATION_DIED},
  {"observerSpawned", SimulationEventType::OBSERVER_SPAWNED},
  {"observerDied", SimulationEventType::OBSERVER_DIED},
  {"occurrenceSpawned", SimulationEventType::OCCURRENCE_SPAWNED},
  {"occurrenceSpawnFailed", SimulationEventType::OCCURRENCE_SPAWN_FAILED},
  {"occurrenceDied", SimulationEventType::OCCURRENCE_DIED},
  {"navigatorSpawned", SimulationEventType::NAVIGATOR_SPAWNED},
  {"navigatorReachedLocation", SimulationEventType::NAVIGATOR_REACHED_LOCATION},
  {"navigatorDied", SimulationEventType::NAVIGATOR_DIED},
  {"nodeSpawned", SimulationEventType::NODE_SPAWNED},
  {"nodeDied", SimulationEventType::NODE_DIED},
};

template<>
const EnumTypeInfo<SimulationEventType>& ofxTCommon::getEnumInfo() {
  return SimulationEventTypeInfo;
}

std::ostream& operator<<(std::ostream& os,
                         const SimulationEventType& value) {
  return os << enumToString(value);
}

AbstractEvent* SimulationEvents::getEvent(SimulationEventType type) {
  switch (type) {
    case SimulationEventType::ANIMATION_SPAWNED:
      return &animationSpawned;
    case SimulationEventType::ANIMATION_DIED:
      return &animationDied;
    case SimulationEventType::OBSERVER_SPAWNED:
      return &observerSpawned;
    case SimulationEventType::OBSERVER_DIED:
      return &observerDied;
    case SimulationEventType::OCCURRENCE_SPAWNED:
      return &occurrenceSpawned;
    case SimulationEventType::OCCURRENCE_SPAWN_FAILED:
      return &occurrenceSpawnFailed;
    case SimulationEventType::OCCURRENCE_DIED:
      return &occurrenceDied;
    case SimulationEventType::NAVIGATOR_SPAWNED:
      return &navigatorSpawned;
    case SimulationEventType::NAVIGATOR_REACHED_LOCATION:
      return &navigatorReachedLocation;
    case SimulationEventType::NAVIGATOR_DIED:
      return &navigatorDied;
    case SimulationEventType::NODE_SPAWNED:
      return &nodeSpawned;
    case SimulationEventType::NODE_DIED:
      return &nodeDied;
    default:
      AppSystem::get().log().control().logWarning("Unsupported event type: " + SimulationEventTypeInfo.toString(type));
      return nullptr;
  }
}

template<>
SimulationEvent<AnimationObject>& SimulationEvents::spawned() {
  return animationSpawned;
}

template<>
SimulationEvent<ObserverEntity>& SimulationEvents::spawned() {
  return observerSpawned;
}

template<>
SimulationEvent<OccurrenceEntity>& SimulationEvents::spawned() {
  return occurrenceSpawned;
}

template<>
SimulationEvent<NavigatorEntity>& SimulationEvents::spawned() {
  return navigatorSpawned;
}

template<>
SimulationEvent<NodeEntity>& SimulationEvents::spawned() {
  return nodeSpawned;
}

template<>
SimulationEvent<AnimationObject>& SimulationEvents::died() {
  return animationDied;
}

template<>
SimulationEvent<ObserverEntity>& SimulationEvents::died() {
  return observerDied;
}

template<>
SimulationEvent<OccurrenceEntity>& SimulationEvents::died() {
  return occurrenceDied;
}

template<>
SimulationEvent<NavigatorEntity>& SimulationEvents::died() {
  return navigatorDied;
}

template<>
SimulationEvent<NodeEntity>& SimulationEvents::died() {
  return nodeDied;
}
