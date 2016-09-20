//
//  ForceNodeBehavior.cpp
//  memory
//
//  Created by tekt on 9/18/16.
//
//

#include "ForceNodeBehavior.h"

void AbstractVortexForceNodeBehavior::cullDeadNodes() {
  _nodes.cullObjects();
}

void AbstractVortexForceNodeBehavior::applyToEntity(ParticleObject *entity) {
  for (const auto& node : _nodes) {
    auto force = calculateForce(*node, entity);
    entity->addForce(force);
  }
}

ofVec3f AbstractVortexForceNodeBehavior::calculateForce(const VortexForceNode &node,
                                                        ParticleObject *entity) {
  const ofVec3f& nodePos = node.position();
  const ofVec3f& entityPos = entity->position();
  auto posDiff = entityPos - nodePos;
  auto dist = posDiff.length();
  if (dist > node.radius()) {
    return ofVec3f::zero();
  }
  posDiff /= node.radius();
  dist = posDiff.length();
  auto magnitude =
  _params.force.evaluate(dist) * _params.magnitude.get();
  auto force = posDiff.getRotated(90, node.velocity());
  return force * magnitude;
}
