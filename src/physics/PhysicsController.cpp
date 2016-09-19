//
//  PhysicsController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "../app/AppParameters.h"
#include "../physics/PhysicsController.h"

PhysicsController::PhysicsController(PhysicsController::Params& params,
                                     Bounds& bounds,
                                     DebugParams& debugParams,
                                     Context& context)
: _params(params)
, _context(context)
, _bounds(bounds)
, _debugParams(debugParams) {}

void PhysicsController::setup() {
  _rebound =
  std::make_shared<BoundsBehavior>(_context,
                                   _params.rebound,
                                   _bounds);
  _observerObserverAttraction =
  std::make_shared<AttractionBehavior<ObserverEntity, ObserverEntity>>(_context,
                                                                       _params.observerObserverAttraction);
  _observerOccurrenceForce = std::make_shared<ObserverOccurrenceForceBehavior>(_context, _params.observerOccurrenceForce);
  _occurrenceOccurrenceForce = std::make_shared<OccurrenceOccurrenceForceBehavior>(_context, _params.occurrenceOccurrenceForce);
  _observerSpatialNoiseForce = std::make_shared<NoiseForceFieldBehavior<ObserverEntity>>(_context, _params.observerSpatialNoiseForce);
  _occurrenceSpatialNoiseForce = std::make_shared<NoiseForceFieldBehavior<OccurrenceEntity>>(_context, _params.occurrenceSpatialNoiseForce);
  _observerAnchorPointAttraction = std::make_shared<AnchorPointBehavior<ObserverEntity>>(_context, _params.observerAnchorPointAttraction);
  _occurrenceAnchorPointAttraction = std::make_shared<AnchorPointBehavior<OccurrenceEntity>>(_context, _params.occurrenceAnchorPointAttraction);
  _observerDamping = std::make_shared<DampingBehavior<ObserverEntity>>(_context, _params.observerDamping);
  _occurrenceDamping = std::make_shared<DampingBehavior<OccurrenceEntity>>(_context, _params.occurrenceDamping);
}

bool PhysicsController::performAction(AppAction action) {
  switch (action) {
    case AppAction::STOP_ALL_ENTITIES:
      stopAllEntities();
      break;
    case AppAction::TOGGLE_SHOW_BOUNDS:
      _debugParams.showBounds.toggle();
      break;
    case AppAction::TOGGLE_SHOW_PHYSICS:
      _debugParams.showPhysics.toggle();
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
  _observerObserverAttraction->applyToWorld();
  _observerOccurrenceForce->applyToWorld();
  _occurrenceOccurrenceForce->applyToWorld();
  _observerSpatialNoiseForce->applyToWorld();
  _occurrenceSpatialNoiseForce->applyToWorld();
  _observerAnchorPointAttraction->applyToWorld();
  _occurrenceAnchorPointAttraction->applyToWorld();
  _observerDamping->applyToWorld();
  _occurrenceDamping->applyToWorld();
  _rebound->applyToWorld();
  for (auto& entity : _context.observers) {
    endEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    endEntityUpdate(entity.get(), _params.occurrences);
  }
}

void PhysicsController::draw() {
  if (_debugParams.showPhysics()) {
    _observerObserverAttraction->debugDraw();
    _observerOccurrenceForce->debugDraw();
    _occurrenceOccurrenceForce->debugDraw();
    _observerSpatialNoiseForce->debugDraw();
    _occurrenceSpatialNoiseForce->debugDraw();
    _observerAnchorPointAttraction->debugDraw();
    _occurrenceAnchorPointAttraction->debugDraw();
    _observerDamping->debugDraw();
    _occurrenceDamping->debugDraw();
    _rebound->debugDraw();
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
