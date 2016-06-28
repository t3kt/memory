//
//  ObserversController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef ObserversController_h
#define ObserversController_h

#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ObjectManager.h"
#include "State.h"
#include "Params.h"
#include "Interval.h"
#include "Events.h"
#include "Bounds.h"
#include "ThresholdRenderer.h"

using ObserverEventArgs = EntityEventArgs<ObserverEntity>;
using ObserverEvent = ofxLiquidEvent<ObserverEventArgs>;

class ObserversController {
public:
  class Params : public ::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    ObserverEntity::Params entities;
    Interval::Params spawnInterval;
    SimpleRandomVectorSupplier initialVelocity;
    ObserverOccurrenceAttraction::Params occurrenceAttraction;
    AbstractThresholdRenderer::Params threshold;
  };
  
  ObserversController(const Params& params, const Bounds& bounds, const State& state);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);
  
  bool registerOccurrence(shared_ptr<OccurrenceEntity> occurrence);

  std::size_t count() const {
    return _observers.size();
  }
  
  ObserverEvent observerSpawned;
  ObserverEvent observerDied;
  ObserverEvent observerRebounded;
private:
  void spawnObserver(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  Interval _spawnInterval;
  ObjectManager<ObserverEntity> _observers;
  shared_ptr<ReboundBehavior<ObserverEntity>> _reboundBehavior;
  shared_ptr<ObserverOccurrenceAttraction> _occurrenceAttraction;
  shared_ptr<ThresholdRenderer<ObserverEntity>> _thresholdRenderer;
};

#endif /* ObserversController_h */
