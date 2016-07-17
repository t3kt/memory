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
                                     Context& context)
: _params(params)
, _context(context)
, _world(context)
, _bounds(bounds)
, _debugParams(debugParams) {}

void PhysicsController::setup() {
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
  entity->updateVelocityAndPosition(_context.state, params.speed());
}

void PhysicsController::update() {
  for (auto& entity : _context.observers) {
    beginEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    beginEntityUpdate(entity.get(), _params.occurrences);
  }
  _observerOccurrenceAttraction->applyToWorld(&_world);
  _occurrenceObserverAttraction->applyToWorld(&_world);
  _observerObserverAttraction->applyToWorld(&_world);
  _observerSpatialNoiseForce->applyToWorld(&_world);
  _occurrenceSpatialNoiseForce->applyToWorld(&_world);
  _observerAnchorPointAttraction->applyToWorld(&_world);
  _occurrenceAnchorPointAttraction->applyToWorld(&_world);
  _observerDamping->applyToWorld(&_world);
  _occurrenceDamping->applyToWorld(&_world);
  _rebound->applyToWorld(&_world);
  for (auto& entity : _context.observers) {
    endEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    endEntityUpdate(entity.get(), _params.occurrences);
  }
}

void PhysicsController::draw() {
  if (_debugParams.showPhysics()) {
    _observerOccurrenceAttraction->debugDraw(&_world);
    _occurrenceObserverAttraction->debugDraw(&_world);
    _observerObserverAttraction->debugDraw(&_world);
    _observerSpatialNoiseForce->debugDraw(&_world);
    _occurrenceSpatialNoiseForce->debugDraw(&_world);
    _observerAnchorPointAttraction->debugDraw(&_world);
    _occurrenceAnchorPointAttraction->debugDraw(&_world);
    _observerDamping->debugDraw(&_world);
    _occurrenceDamping->debugDraw(&_world);
    _rebound->debugDraw(&_world);
  }
}

void PhysicsController::stopAllEntities() {
  for (auto& entity : _context.observers) {
    entity->setVelocity(ofVec3f::zero());
  }
  for (auto& entity : _context.occurrences) {
    entity->setVelocity(ofVec3f::zero());
  }
}
