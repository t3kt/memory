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
  addBehavior<AttractionBehavior<ObserverEntity, ObserverEntity>>(_context, _params.observerObserverAttraction);
  addBehavior<ObserverOccurrenceForceBehavior>(_context, _params.observerOccurrenceForce);
  addBehavior<OccurrenceOccurrenceForceBehavior>(_context, _params.occurrenceOccurrenceForce);
  addBehavior<NoiseForceFieldBehavior<ObserverEntity>>(_context, _params.observerSpatialNoiseForce);
  addBehavior<NoiseForceFieldBehavior<OccurrenceEntity>>(_context, _params.occurrenceSpatialNoiseForce);
  addBehavior<AnchorPointBehavior<ObserverEntity>>(_context, _params.observerAnchorPointAttraction);
  addBehavior<AnchorPointBehavior<OccurrenceEntity>>(_context, _params.occurrenceAnchorPointAttraction);
  addBehavior<DampingBehavior<ObserverEntity>>(_context, _params.observerDamping);
  addBehavior<DampingBehavior<OccurrenceEntity>>(_context, _params.occurrenceDamping);
  addBehavior<BoundsBehavior>(_context, _params.rebound, _bounds);
  auto behavior =
  addBehavior<VortexForceNodeBehavior<ObserverEntity>>(_context, _params.observerVortexNodes);
  for (int i = 0; i < 2; i++) {
    auto pos = _bounds.randomPoint();
    behavior->spawnNode(pos);
  }
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
  for (auto& behavior : _behaviors) {
    behavior->applyToWorld();
  }
  for (auto& entity : _context.observers) {
    endEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    endEntityUpdate(entity.get(), _params.occurrences);
  }
}

void PhysicsController::draw() {
  if (_debugParams.showPhysics()) {
    for (auto& behavior : _behaviors) {
      behavior->debugDraw();
    }
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
