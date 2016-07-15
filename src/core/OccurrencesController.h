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
#include "Events.h"
#include "Bounds.h"
#include "Colors.h"
#include "EntityRenderer.h"
#include "SimulationEvents.h"
#include "Spawner.h"

class IntervalOccurrenceSpawner;
class RateOccurrenceSpawner;

class OccurrencesController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(radius
          .setKey("radius")
          .setName("Radius Range")
          .setParamValuesAndDefaults(0.4, 1.3)
          .setParamRanges(0, 4));
      add(spawner
          .setKey("spawner")
          .setName("Spawner"));
      add(rateSpawner
          .setKey("rateSpawner")
          .setName("Rate Spawner"));
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
    }

    RandomValueSupplier<float> radius;
    IntervalSpawner::Params spawner;
    RateSpawner::Params rateSpawner;
    SimpleRandomVectorSupplier initialVelocity;
    OccurrenceRenderer::Params renderer;
    ObserverOccurrenceConnectorRenderer::Params connectorRenderer;
  };
  
  OccurrencesController(const Params& params,
                        const Bounds& bounds,
                        ObserversController& observers,
                        const State& state,
                        SimulationEvents& events);
  
  void setup(const State& state, const ColorTheme& colors);
  void update(State& state);
  void draw(const State& state);

  void spawnOccurrences(int count, const State& state);

  ObjectManager<OccurrenceEntity>& entities() { return _occurrences; }
  const ObjectManager<OccurrenceEntity>& entities() const {
    return _occurrences;
  }
  
private:
  void spawnRandomOccurrence(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  SimulationEvents& _events;
  ObserversController& _observers;
  ObjectManager<OccurrenceEntity> _occurrences;
  std::shared_ptr<IntervalOccurrenceSpawner> _spawner;
  std::shared_ptr<RateOccurrenceSpawner> _rateSpawner;
  std::shared_ptr<OccurrenceRenderer> _renderer;
  std::shared_ptr<ObserverOccurrenceConnectorRenderer> _observerOccurrenceConnectorRenderer;

  friend class IntervalOccurrenceSpawner;
  friend class RateOccurrenceSpawner;
};

#endif /* OccurrencesController_h */
