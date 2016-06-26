//
//  ObserversController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef ObserversController_h
#define ObserversController_h

#include <ofEvent.h>
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ObjectManager.h"
#include "State.h"
#include "Params.h"
#include "Interval.h"
#include "Events.h"

class ObserversController {
public:
  class Params : public ::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    ObserverEntity::Params entities;
    Interval::Params spawnInterval;
  };
  
  ObserversController(const Params& params, const State& state);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);
  
  bool registerOccurrence(shared_ptr<OccurrenceEntity> occurrence);
  
  ofEvent<EntityEventArgs<ObserverEntity>> observerSpawned;
  ofEvent<EntityEventArgs<ObserverEntity>> observerDied;
private:
  void spawnObserver(const State& state);
  
  const Params& _params;
  Interval _spawnInterval;
  ObjectManager<ObserverEntity> _observers;
};

#endif /* ObserversController_h */
