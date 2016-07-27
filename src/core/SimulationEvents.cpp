//
//  SimulationEvents.cpp
//  memory
//
//  Created by tekt on 7/27/16.
//
//

#include "SimulationEvents.h"

EnumTypeInfo<SimulationEventType> SimulationEventTypeType {
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
};

std::ostream& operator<<(std::ostream& os,
                         const SimulationEventType& value) {
  return os << SimulationEventTypeType.toString(value);
}
