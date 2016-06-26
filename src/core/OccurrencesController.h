//
//  OccurrencesController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef OccurrencesController_h
#define OccurrencesController_h

#include "OccurrenceEntity.h"
#include "ObserversController.h"
#include "ObjectManager.h"
#include "State.h"
#include "Params.h"
#include "Interval.h"
#include "Events.h"

using OccurrenceEventArgs = EntityEventArgs<OccurrenceEntity>;
using OccurrenceEvent = ofxLiquidEvent<OccurrenceEventArgs>;

class OccurrencesController {
public:
  class Params : public ::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    OccurrenceEntity::Params entities;
    Interval::Params spawnInterval;
  };
  
  OccurrencesController(const Params& params, ObserversController& observers, const State& state);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);
  
  OccurrenceEvent occurrenceSpawned;
  OccurrenceEvent occurrenceDied;
  
private:
  void spawnOccurrence(const State& state);
  
  const Params& _params;
  Interval _spawnInterval;
  ObserversController& _observers;
  ObjectManager<OccurrenceEntity> _occurrences;
};

#endif /* OccurrencesController_h */
