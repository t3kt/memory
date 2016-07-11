//
//  SimulationEvents.h
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#ifndef SimulationEvents_h
#define SimulationEvents_h

#include <ofxLiquidEvent.h>
#include "Events.h"

class SimulationEvents {
public:
  AnimationEvent animationSpawned;
  AnimationEvent animationDied;

  ObserverEvent observerSpawned;
  ObserverEvent observerDied;

  OccurrenceEvent occurrenceSpawned;
  OccurrenceEvent occurrenceSpawnFailed;
  OccurrenceEvent occurrenceDied;
};

#endif /* SimulationEvents_h */
