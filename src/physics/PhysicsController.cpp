//
//  PhysicsController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "AppParameters.h"
#include "PhysicsController.h"

PhysicsController::PhysicsController(PhysicsController::Params& params,
                                     Bounds& bounds,
                                     DebugParams& debugParams,
                                     const State& state)
: _params(params)
, _bounds(bounds)
, _debugParams(debugParams)
, _state(state) {}

void PhysicsController::setup(ObserversController& observers,
                              OccurrencesController& occurrences) {
  _rebound = std::make_shared<BoundsBehavior>(_params.rebound, _bounds);
  _observerOccurrenceAttraction = std::make_shared<AttractionBehavior<ObserverEntity, OccurrenceEntity>>(_params.observerOccurrenceAttraction);
  _occurrenceObserverAttraction = std::make_shared<AttractionBehavior<OccurrenceEntity, ObserverEntity>>(_params.occurrenceObserverAttraction);
  _observerObserverAttraction = std::make_shared<AttractionBehavior<ObserverEntity, ObserverEntity>>(_params.observerObserverAttraction);
  _observerSpatialNoiseForce = std::make_shared<NoiseForceFieldBehavior<ObserverEntity>>(_params.observerSpatialNoiseForce);
  _occurrenceSpatialNoiseForce = std::make_shared<NoiseForceFieldBehavior<OccurrenceEntity>>(_params.occurrenceSpatialNoiseForce);
  _observerAnchorPointAttraction = std::make_shared<AnchorPointBehavior<ObserverEntity>>(_params.observerAnchorPointAttraction);
  _occurrenceAnchorPointAttraction = std::make_shared<AnchorPointBehavior<OccurrenceEntity>>(_params.occurrenceAnchorPointAttraction);
  _observerDamping = std::make_shared<DampingBehavior<ObserverEntity>>(_params.observerDamping);
  _occurrenceDamping = std::make_shared<DampingBehavior<OccurrenceEntity>>(_params.occurrenceDamping);

  _world = std::make_shared<PhysicsWorld>(_state,
                                          observers,
                                          occurrences);

  registerAsActionHandler();
}

bool PhysicsController::performAction(AppAction action) {
  switch (action) {
    case AppAction::STOP_ALL_ENTITIES:
      stopAllEntities();
      break;
    case AppAction::TOGGLE_SHOW_BOUNDS:
      toggleBoolParam(_debugParams.showBounds);
      break;
    case AppAction::TOGGLE_SHOW_PHYSICS:
      toggleBoolParam(_debugParams.showPhysics);
      break;
    default:
      return false;
  }
  return true;
}

void PhysicsController::beginEntityUpdate(ParticleObject *entity,
                                          const EntityPhysicsParams& params) {
  if (!entity->alive()) {
    return;
  }
  entity->resetForce();
}

void PhysicsController::endEntityUpdate(ParticleObject *entity,
                                        const EntityPhysicsParams& params) {
  if (!entity->alive()) {
    return;
  }
  entity->updateVelocityAndPosition(_world->state(), params.speed());
}

void PhysicsController::update() {
  for (auto& entity : _world->observers()) {
    beginEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _world->occurrences()) {
    beginEntityUpdate(entity.get(), _params.occurrences);
  }
  auto world = _world.get();
  _observerOccurrenceAttraction->applyToWorld(world);
  _occurrenceObserverAttraction->applyToWorld(world);
  _observerObserverAttraction->applyToWorld(world);
  _observerSpatialNoiseForce->applyToWorld(world);
  _occurrenceSpatialNoiseForce->applyToWorld(world);
  _observerAnchorPointAttraction->applyToWorld(world);
  _occurrenceAnchorPointAttraction->applyToWorld(world);
  _observerDamping->applyToWorld(world);
  _occurrenceDamping->applyToWorld(world);
  _rebound->applyToWorld(world);
  for (auto& entity : _world->observers()) {
    endEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _world->occurrences()) {
    endEntityUpdate(entity.get(), _params.occurrences);
  }
}

void PhysicsController::draw() {
  if (_debugParams.showPhysics()) {
    auto world = _world.get();
    _observerOccurrenceAttraction->debugDraw(world);
    _occurrenceObserverAttraction->debugDraw(world);
    _observerObserverAttraction->debugDraw(world);
    _observerSpatialNoiseForce->debugDraw(world);
    _occurrenceSpatialNoiseForce->debugDraw(world);
    _observerAnchorPointAttraction->debugDraw(world);
    _occurrenceAnchorPointAttraction->debugDraw(world);
    _observerDamping->debugDraw(world);
    _occurrenceDamping->debugDraw(world);
    _rebound->debugDraw(world);
  }
}

void PhysicsController::stopAllEntities() {
  for (auto& entity : _world->observers()) {
    entity->setVelocity(ofVec3f::zero());
  }
  for (auto& entity : _world->occurrences()) {
    entity->setVelocity(ofVec3f::zero());
  }
}
