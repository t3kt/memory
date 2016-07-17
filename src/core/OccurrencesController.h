//
//  OccurrencesController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef OccurrencesController_h
#define OccurrencesController_h

#include "AppActions.h"
#include "OccurrenceEntity.h"
#include "ObserversController.h"
#include "ObjectManager.h"
#include "State.h"
#include "Params.h"
#include "Events.h"
#include "Bounds.h"
#include "Colors.h"
#include "Context.h"
#include "EntityRenderer.h"
#include "SimulationEvents.h"
#include "Spawner.h"

class IntervalOccurrenceSpawner;
class RateOccurrenceSpawner;

class OccurrencesController
: public AppActionHandler {
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
          .setName("Inteval Spawner"));
      add(rateSpawner
          .setRateRange(0, 40)
          .setRateValueAndDefault(4)
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
      add(occurrenceConnectorRenderer
          .setKey("occurrenceConnectorRenderer")
          .setName("Occurrence Connector Renderer"));
    }

    RandomValueSupplier<float> radius;
    IntervalSpawner::Params spawner;
    RateSpawner::Params rateSpawner;
    SimpleRandomVectorSupplier initialVelocity;
    OccurrenceRenderer::Params renderer;
    ObserverOccurrenceConnectorRenderer::Params connectorRenderer;
    OccurrenceOccurrenceConnectorRenderer::Params occurrenceConnectorRenderer;
  };
  
  OccurrencesController(const Params& params,
                        const Bounds& bounds,
                        ObserversController& observers,
                        Context& context,
                        SimulationEvents& events);
  
  void setup(const State& state, const ColorTheme& colors);
  void update(State& state);
  void draw(const State& state);

  void spawnOccurrences(int count, const State& state);

  ObjectManager<OccurrenceEntity>& entities() { return _occurrences; }
  const ObjectManager<OccurrenceEntity>& entities() const {
    return _occurrences;
  }

  bool performAction(AppAction action) override;
  
private:
  void spawnRandomOccurrence(const State& state);
  
  const Params& _params;
  const Bounds& _bounds;
  SimulationEvents& _events;
  ObserversController& _observers;
  ObjectManager<OccurrenceEntity>& _occurrences;
  std::shared_ptr<IntervalOccurrenceSpawner> _spawner;
  std::shared_ptr<RateOccurrenceSpawner> _rateSpawner;
  std::shared_ptr<OccurrenceRenderer> _renderer;
  std::shared_ptr<ObserverOccurrenceConnectorRenderer> _observerOccurrenceConnectorRenderer;
  std::shared_ptr<OccurrenceOccurrenceConnectorRenderer> _occurrenceOccurrenceConnectorRenderer;

  friend class IntervalOccurrenceSpawner;
  friend class RateOccurrenceSpawner;
};

#endif /* OccurrencesController_h */
