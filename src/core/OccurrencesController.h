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
#include "Status.h"
#include "Colors.h"
#include "Renderer.h"

using OccurrenceEventArgs = EntityEventArgs<OccurrenceEntity>;
using OccurrenceEvent = ofxLiquidEvent<OccurrenceEventArgs>;

class OccurrencesController
: public StatusInfoProvider {
public:
  class Params : public ::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;
    
    void initPanel(ofxGuiGroup& panel) override;
    
    OccurrenceEntity::Params entities;
    Interval::Params spawnInterval;
    SimpleRandomVectorSupplier initialVelocity;
    OccurrenceObserverAttraction::Params observerAttraction;
    AbstractSpatialNoiseForce::Params spatialNoiseForce;
    OccurrenceRenderer::Params renderer;
  };
  
  OccurrencesController(const Params& params, const Bounds& bounds, ObserversController& observers, const State& state, const ColorTheme& colors);
  
  void setup(const State& state);
  void update(const State& state);
  void draw(const State& state);

  void spawnOccurrences(int count, const State& state);

  std::size_t count() const {
    return _occurrences.size();
  }

  const StatusInfo& getStatusInfo() const override {
    return _status;
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
  shared_ptr<ReboundBehavior<OccurrenceEntity>> _reboundBehavior;
  shared_ptr<OccurrenceObserverAttraction> _observerAttraction;
  shared_ptr<SpatialNoiseForce<OccurrenceEntity>> _spatialNoiseForce;
  StatusInfo _status;
  std::size_t STATUS_COUNT;
};

#endif /* OccurrencesController_h */
