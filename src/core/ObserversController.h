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
#include "SimulationEvents.h"

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
      add(connectorRenderer
          .setKey("connectorRenderer")
          .setName("Connector Renderer"));
      add(threshold
          .setKey("threshold")
          .setName("Threshold"));
    }

    RandomValueSupplier<float> lifetime;
    Interval::Params spawnInterval;
    SimpleRandomVectorSupplier initialVelocity;
    ObserverRenderer::Params renderer;
    ObserverObserverConnectorRenderer::Params connectorRenderer;
    AbstractThresholdRenderer::Params threshold;
  };
  
  ObserversController(const Params& params,
                      const Bounds& bounds,
                      const State& state,
                      SimulationEvents& events);
  
  void setup(const State& state, const ColorTheme& colors);
  void update(State& state);
  void draw(const State& state);
  
  bool registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void spawnObservers(int count, const State& state);

  void killObservers(int count);

  ObjectManager<ObserverEntity>& entities() { return _observers; }
  const ObjectManager<ObserverEntity>& entities() const {
    return _observers;
  }
  
private:
  void spawnObserver(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  SimulationEvents& _events;
  Interval _spawnInterval;
  ObjectManager<ObserverEntity> _observers;
  std::shared_ptr<ObserverRenderer> _observerRenderer;
  std::shared_ptr<ObserverObserverConnectorRenderer> _observerConnectorRenderer;
  std::shared_ptr<ThresholdRenderer<ObserverEntity>> _thresholdRenderer;
};

#endif /* ObserversController_h */
