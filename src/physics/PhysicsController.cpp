//
//  PhysicsController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "PhysicsController.h"
#include "ObserversController.h"
#include "OccurrencesController.h"

PhysicsController::PhysicsController(const PhysicsController::Params& params,
                                     const Bounds& bounds,
                                     const State& state)
: _params(params)
, _bounds(bounds)
, _state(state) {}

void PhysicsController::setup(ObserversController& observers,
                              OccurrencesController& occurrences) {
  _rebound = std::make_shared<BoundsBehavior>(_params.rebound, _bounds);
  _observerOccurrenceAttraction = std::make_shared<AttractionBehavior<ObserverEntity, OccurrenceEntity>>(_params.observerOccurrenceAttraction);
  _occurrenceObserverAttraction = std::make_shared<AttractionBehavior<OccurrenceEntity, ObserverEntity>>(_params.occurrenceObserverAttraction);
  _observerSpatialNoiseForce = std::make_shared<SpatialNoiseForceBehavior<ObserverEntity>>(_params.observerSpatialNoiseForce);
  _occurrenceSpatialNoiseForce = std::make_shared<SpatialNoiseForceBehavior<OccurrenceEntity>>(_params.occurrenceSpatialNoiseForce);

  _world = std::make_shared<PhysicsWorld>(_state,
                                          observers,
                                          occurrences);
}

void PhysicsController::update() {
  auto world = _world.get();
  _observerOccurrenceAttraction->applyToWorld(world);
  _occurrenceObserverAttraction->applyToWorld(world);
  _observerSpatialNoiseForce->applyToWorld(world);
  _occurrenceSpatialNoiseForce->applyToWorld(world);
  _rebound->applyToWorld(world);
}
