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
#include "Bounds.h"
#include "Behavior.h"
#include "Colors.h"
#include "EntityRenderer.h"

using OccurrenceEventArgs = EntityEventArgs<OccurrenceEntity>;
using OccurrenceEvent = ofxLiquidEvent<OccurrenceEventArgs>;

class OccurrencesController {
public:
  class Params : public ::Params {
  public:
    Params();
    
    OccurrenceEntity::Params entities;
    Interval::Params spawnInterval;
    SimpleRandomVectorSupplier initialVelocity;
    OccurrenceObserverAttraction::Params observerAttraction;
    AbstractSpatialNoiseForce::Params spatialNoiseForce;
    OccurrenceRenderer::Params renderer;
    ObserverOccurrenceConnectorRenderer::Params connectorRenderer;
  };
  
  OccurrencesController(const Params& params, const Bounds& bounds, ObserversController& observers, const State& state, const ColorTheme& colors);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);

  void spawnOccurrences(int count, const State& state);

  std::size_t count() const {
    return _occurrences.size();
  }
  
  OccurrenceEvent occurrenceSpawned;
  OccurrenceEvent occurrenceSpawnFailed;
  OccurrenceEvent occurrenceDied;
  
private:
  void spawnOccurrence(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  const ColorTheme& _colors;
  Interval _spawnInterval;
  ObserversController& _observers;
  ObjectManager<OccurrenceEntity> _occurrences;
  shared_ptr<OccurrenceRenderer> _renderer;
  shared_ptr<ObserverOccurrenceConnectorRenderer> _observerOccurrenceConnectorRenderer;
  shared_ptr<ReboundBehavior<OccurrenceEntity>> _reboundBehavior;
  shared_ptr<OccurrenceObserverAttraction> _observerAttraction;
  shared_ptr<SpatialNoiseForce<OccurrenceEntity>> _spatialNoiseForce;
};

#endif /* OccurrencesController_h */
