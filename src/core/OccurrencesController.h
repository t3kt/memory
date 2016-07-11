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
#include "Colors.h"
#include "EntityRenderer.h"
#include "SimulationEvents.h"

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
    }

    RandomValueSupplier<float> radius;
    Interval::Params spawnInterval;
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
  Interval _spawnInterval;
  SimulationEvents& _events;
  ObserversController& _observers;
  ObjectManager<OccurrenceEntity> _occurrences;
  std::shared_ptr<OccurrenceRenderer> _renderer;
  std::shared_ptr<ObserverOccurrenceConnectorRenderer> _observerOccurrenceConnectorRenderer;
};

#endif /* OccurrencesController_h */
