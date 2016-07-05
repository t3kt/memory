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
#include "EntityRenderer.h"
#include "Status.h"
#include "Colors.h"

using ObserverEventArgs = EntityEventArgs<ObserverEntity>;
using ObserverEvent = ofxLiquidEvent<ObserverEventArgs>;

class ObserversController
: public StatusInfoProvider {
public:
  class Params : public ::Params {
  public:
    Params();
    
    ObserverEntity::Params entities;
    Interval::Params spawnInterval;
    SimpleRandomVectorSupplier initialVelocity;
    ObserverOccurrenceAttraction::Params occurrenceAttraction;
    AbstractSpatialNoiseForce::Params spatialNoiseForce;
    ObserverRenderer::Params renderer;
    AbstractThresholdRenderer::Params threshold;
  };
  
  ObserversController(const Params& params, const Bounds& bounds, const State& state, const ColorTheme& colors);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);
  
  bool registerOccurrence(shared_ptr<OccurrenceEntity> occurrence);

  void spawnObservers(int count, const State& state);

  std::size_t count() const {
    return _observers.size();
  }

  const StatusInfo& getStatusInfo() const { return _status; }
  
  ObserverEvent observerSpawned;
  ObserverEvent observerDied;
  ObserverEvent observerRebounded;
private:
  void spawnObserver(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  const ColorTheme& _colors;
  Interval _spawnInterval;
  ObjectManager<ObserverEntity> _observers;
  shared_ptr<ObserverRenderer> _observerRenderer;
  shared_ptr<ReboundBehavior<ObserverEntity>> _reboundBehavior;
  shared_ptr<ObserverOccurrenceAttraction> _occurrenceAttraction;
  shared_ptr<SpatialNoiseForce<ObserverEntity>> _spatialNoiseForce;
  shared_ptr<ThresholdRenderer<ObserverEntity>> _thresholdRenderer;
  StatusInfo _status;
  std::size_t STATUS_COUNT;
};

#endif /* ObserversController_h */
