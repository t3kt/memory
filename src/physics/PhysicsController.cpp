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
, _bounds(bounds)
, _debugParams(debugParams) {}

void PhysicsController::setup() {
  _rebound = std::make_shared<BoundsBehavior>(_params.rebound, _bounds);
  _observerObserverAttraction = std::make_shared<AttractionBehavior<ObserverEntity, ObserverEntity>>(_params.observerObserverAttraction);
  _observerOccurrenceForce = std::make_shared<ObserverOccurrenceForceBehavior>(_params.observerOccurrenceForce);
  _occurrenceOccurrenceForce = std::make_shared<OccurrenceOccurrenceForceBehavior>(_params.occurrenceOccurrenceForce);
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
  _observerObserverAttraction->applyToWorld(_context);
  _observerOccurrenceForce->applyToWorld(_context);
  _occurrenceOccurrenceForce->applyToWorld(_context);
  _observerSpatialNoiseForce->applyToWorld(_context);
  _occurrenceSpatialNoiseForce->applyToWorld(_context);
  _observerAnchorPointAttraction->applyToWorld(_context);
  _occurrenceAnchorPointAttraction->applyToWorld(_context);
  _observerDamping->applyToWorld(_context);
  _occurrenceDamping->applyToWorld(_context);
  _rebound->applyToWorld(_context);
  for (auto& entity : _context.observers) {
    endEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    endEntityUpdate(entity.get(), _params.occurrences);
  }
}

void PhysicsController::draw() {
  if (_debugParams.showPhysics()) {
    _observerObserverAttraction->debugDraw(_context);
    _observerOccurrenceForce->debugDraw(_context);
    _occurrenceOccurrenceForce->debugDraw(_context);
    _observerSpatialNoiseForce->debugDraw(_context);
    _occurrenceSpatialNoiseForce->debugDraw(_context);
    _observerAnchorPointAttraction->debugDraw(_context);
    _occurrenceAnchorPointAttraction->debugDraw(_context);
    _observerDamping->debugDraw(_context);
    _occurrenceDamping->debugDraw(_context);
    _rebound->debugDraw(_context);
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
