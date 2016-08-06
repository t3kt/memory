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
#include "ConnectorRenderer.h"
#include "OccurrenceEntity.h"
#include "OccurrenceRenderer.h"
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

class OccurrencesController;

class IntervalOccurrenceSpawner
: public IntervalSpawner<IntervalSpawnerParams> {
public:
  IntervalOccurrenceSpawner(OccurrencesController& controller);
protected:
  void spawnEntities(Context& context) override;

  OccurrencesController& _controller;
};

class RateOccurrenceSpawner
: public RateSpawner {
public:
  RateOccurrenceSpawner(OccurrencesController& controller);

protected:
  void spawnEntities(Context& context, int count) override;

  OccurrencesController& _controller;
};

class OccurrencesController
: public AppActionHandler {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(radius
          .setKey("radius")
          .setName("Radius Range")
          .setParamValuesAndDefaults(0, 80)
          .setParamRanges(0, 400));
      add(spawner
          .setKey("spawner")
          .setName("Inteval Spawner"));
      add(rateSpawner
          .setRateRange(0, 5)
          .setRateValueAndDefault(0.5)
          .setKey("rateSpawner")
          .setName("Rate Spawner"));
      add(initialVelocity
          .setKey("initialVelocity")
          .setName("Initial Velocity")
          .setParamValuesAndDefaults(0, 2)
          .setParamRanges(0, 20));
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
    IntervalSpawnerParams spawner;
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
  
  void setup();
  void update();
  void draw();

  void spawnOccurrences(int count);

  ObjectManager<OccurrenceEntity>& entities() { return _occurrences; }
  const ObjectManager<OccurrenceEntity>& entities() const {
    return _occurrences;
  }

  bool performAction(AppAction action) override;
  
private:
  void spawnRandomOccurrence();
  
  const Params& _params;
  Context& _context;
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
  friend class DescendantOccurrenceSpawner;
};

#endif /* OccurrencesController_h */
