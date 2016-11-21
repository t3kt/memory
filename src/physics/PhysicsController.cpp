//
//  PhysicsController.cpp
//

#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
#include "../physics/PhysicsController.h"

PhysicsController::PhysicsController(PhysicsController::Params& params,
                                     DebugParams& debugParams,
                                     Context& context)
: _params(params)
, _context(context)
, _debugParams(debugParams) {}

void PhysicsController::setup() {
  _bounds =
  std::make_shared<BoundsController>(_params.bounds, _debugParams);
  _bounds->setup();
  _behaviors.add<AttractionBehavior<ObserverEntity, ObserverEntity>>(_context, _params.observerObserverAttraction);
  _behaviors.add<ObserverOccurrenceForceBehavior>(_context, _params.observerOccurrenceForce);
  _behaviors.add<OccurrenceOccurrenceForceBehavior>(_context, _params.occurrenceOccurrenceForce);
  _behaviors.add<NoiseForceFieldBehavior<ObserverEntity>>(_context, _params.spatialNoiseForce);
  _behaviors.add<NoiseForceFieldBehavior<OccurrenceEntity>>(_context, _params.spatialNoiseForce);
  _behaviors.add<AnchorPointBehavior<ObserverEntity>>(_context, _params.observerAnchorPointAttraction);
  _behaviors.add<AnchorPointBehavior<OccurrenceEntity>>(_context, _params.occurrenceAnchorPointAttraction);
  _behaviors.add<DampingBehavior<ObserverEntity>>(_context, _params.damping);
  _behaviors.add<DampingBehavior<OccurrenceEntity>>(_context, _params.damping);
  _behaviors.add<BoundsBehavior>(_context, _params.rebound, *_bounds);
  auto behavior =
  _behaviors.add<VortexForceNodeBehavior<ObserverEntity>>(_context, _params.vortexNodes);
  for (int i = 0; i < 2; i++) {
    auto pos = _bounds->randomPoint();
    behavior->spawnNode(pos);
  }

  AppSystem::get().commands()
  .registerCommand("stopEntities", "Stop All Entities", [&](const CommandArgs&) {
    stopAllEntities();
    return true;
  })
  .withButton(true)
  .withKeyMapping('x');
  AppSystem::get().commands()
  .registerCommand("toggleShowPhysics", "Toggle Show Physics", [&](const CommandArgs&) {
    _debugParams.showPhysics.toggle();
    return true;
  })
  .withButton(true)
  .withKeyMapping('p');
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
  entity->updateVelocityAndPosition(_context.physicsState, params.speed());
}

void PhysicsController::update() {
  for (auto& entity : _context.observers) {
    beginEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    beginEntityUpdate(entity.get(), _params.occurrences);
  }
  _behaviors.update();
  for (auto& entity : _context.observers) {
    endEntityUpdate(entity.get(), _params.observers);
  }
  for (auto& entity : _context.occurrences) {
    endEntityUpdate(entity.get(), _params.occurrences);
  }
}

void PhysicsController::draw() {
  _bounds->draw();
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
