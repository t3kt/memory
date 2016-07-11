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
#include "Colors.h"

using ObserverEventArgs = EntityEventArgs<ObserverEntity>;
using ObserverEvent = ofxLiquidEvent<ObserverEventArgs>;

class ObserversController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(lifetime
          .setKey("lifetime")
          .setName("Lifetime Range")
          .setParamValuesAndDefaults(1, 4)
          .setParamRanges(0, 240));
      add(spawnInterval
          .setKey("spawnInterval")
          .setName("Spawning"));
      add(initialVelocity
          .setKey("initialVelocity")
          .setName("Initial Velocity")
          .setParamValuesAndDefaults(0, 0.01)
          .setParamRanges(0, 0.1));
      add(renderer
          .setKey("renderer")
          .setName("Renderer"));
      add(threshold
          .setKey("threshold")
          .setName("Threshold"));
    }

    RandomValueSupplier<float> lifetime;
    Interval::Params spawnInterval;
    SimpleRandomVectorSupplier initialVelocity;
    ObserverRenderer::Params renderer;
    AbstractThresholdRenderer::Params threshold;
  };
  
  ObserversController(const Params& params, const Bounds& bounds, const State& state);
  
  void setup(const State& state, const ColorTheme& colors);
  void update(State& state);
  void draw(const State& state);
  
  bool registerOccurrence(shared_ptr<OccurrenceEntity> occurrence);

  void spawnObservers(int count, const State& state);

  void killObservers(int count);

  ObjectManager<ObserverEntity>& entities() { return _observers; }
  const ObjectManager<ObserverEntity>& entities() const {
    return _observers;
  }

  std::size_t count() const {
    return _observers.size();
  }
  
  ObserverEvent observerSpawned;
  ObserverEvent observerDied;
private:
  void spawnObserver(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  Interval _spawnInterval;
  ObjectManager<ObserverEntity> _observers;
  shared_ptr<ObserverRenderer> _observerRenderer;
  shared_ptr<ThresholdRenderer<ObserverEntity>> _thresholdRenderer;
};

#endif /* ObserversController_h */
