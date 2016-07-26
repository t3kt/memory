//
//  ObserversController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef ObserversController_h
#define ObserversController_h

#include "AppActions.h"
#include "ConnectorRenderer.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ObjectManager.h"
#include "ObserverRenderer.h"
#include "State.h"
#include "Params.h"
#include "Events.h"
#include "Bounds.h"
#include "ThresholdRenderer.h"
#include "EntityRenderer.h"
#include "Colors.h"
#include "Context.h"
#include "SimulationEvents.h"
#include "Spawner.h"

class IntervalObserverSpawner;
class RateObserverSpawner;

class ObserversController
: public AppActionHandler {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(lifetime
          .setKey("lifetime")
          .setName("Lifetime Range")
          .setParamValuesAndDefaults(1, 4)
          .setParamRanges(0, 240));
      add(spawner
          .setKey("spawner")
          .setName("Interval Spawner"));
      add(rateSpawner
          .setRateRange(0, 5)
          .setRateValueAndDefault(0.5)
          .setKey("rateSpawner")
          .setName("Rate Spawner"));
      add(initialVelocity
          .setKey("initialVelocity")
          .setName("Initial Velocity")
          .setParamValuesAndDefaults(0, 4)
          .setParamRanges(0, 20));
      add(renderer
          .setKey("renderer")
          .setName("Renderer"));
      add(instancedRenderer
          .setKey("instancedRenderer")
          .setName("Instanced Renderer"));
      add(connectorRenderer
          .setKey("connectorRenderer")
          .setName("Connector Renderer"));
      add(threshold
          .setKey("threshold")
          .setName("Threshold"));
    }

    RandomValueSupplier<float> lifetime;
    IntervalSpawner::Params spawner;
    RateSpawner::Params rateSpawner;
    SimpleRandomVectorSupplier initialVelocity;
    ObserverRenderer::Params renderer;
    InstancedObserverRenderer::Params instancedRenderer;
    ObserverObserverConnectorRenderer::Params connectorRenderer;
    AbstractThresholdRenderer::Params threshold;
  };
  
  ObserversController(const Params& params,
                      const Bounds& bounds,
                      Context& context,
                      SimulationEvents& events);
  
  void setup(const ColorTheme& colors);
  void update();
  void draw();
  
  bool registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void spawnObservers(int count);

  void killObservers(int count);

  ObjectManager<ObserverEntity>& entities() { return _observers; }
  const ObjectManager<ObserverEntity>& entities() const {
    return _observers;
  }

  bool performAction(AppAction action) override;
  
private:
  void spawnRandomObserver();
  
  const Params& _params;
  Context& _context;
  const Bounds& _bounds;
  SimulationEvents& _events;
  ObjectManager<ObserverEntity>& _observers;
  std::shared_ptr<IntervalObserverSpawner> _spawner;
  std::shared_ptr<RateObserverSpawner> _rateSpawner;
  std::shared_ptr<ObserverRenderer> _observerRenderer;
  std::shared_ptr<InstancedObserverRenderer> _instancedObserverRenderer;
  std::shared_ptr<ObserverObserverConnectorRenderer> _observerConnectorRenderer;
  std::shared_ptr<ThresholdRenderer<ObserverEntity>> _thresholdRenderer;

  friend class IntervalObserverSpawner;
  friend class RateObserverSpawner;
};

#endif /* ObserversController_h */
