//
//  ObserversController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef ObserversController_h
#define ObserversController_h

#include <functional>
#include "ObserverEntity.h"
#include "ObjectManager.h"
#include "State.h"
#include "Params.h"
#include "Interval.h"

class ObserversController {
public:
  class Params : public ::Params {
  public:
    Params();
    
    ObserverEntity::Params entities;
    Interval::Params spawnInterval;
  };
  
  ObserversController(const Params& params, const State& state);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);
  
  void performAction(std::function<void(shared_ptr<ObserverEntity>)> action);
private:
  void spawnObserver(const State& state);
  
  const Params& _params;
  Interval _spawnInterval;
  ObjectManager<ObserverEntity> _observers;
};

#endif /* ObserversController_h */
