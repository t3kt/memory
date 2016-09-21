//
//  ForceNodeBehavior.cpp
//  memory
//
//  Created by tekt on 9/18/16.
//
//

#include <ofMain.h>
#include "ForceNodeBehavior.h"

std::shared_ptr<VortexForceNode>
AbstractVortexForceNodeBehavior::spawnNode(ofVec3f pos) {
  auto radius = _params.radius.getValue();
  auto node = std::make_shared<VortexForceNode>(pos, radius);
  _nodes.add(node);
  _context.nodes.add(node);
  return node;
}

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
  posDiff.normalize();
  dist /= node.radius();
  auto magnitude =
  _params.force.evaluate(dist) * _params.magnitude.get();
  auto axis = node.velocity();
  axis = ofVec3f(1, 0, 0);//!!!!!!!!!!!!!!!!!!!!!!
  auto force = posDiff.getRotated(90, axis);
  return force * magnitude;
}

void AbstractVortexForceNodeBehavior::debugDrawEntity(ParticleObject *entity) {
  for (const auto& node : _nodes) {
    auto force = calculateForce(*node, entity);
    if (force.length() == 0) {
      continue;
    }
    drawForceArrow(entity->position(), force);
  }
}

void AbstractVortexForceNodeBehavior::debugDrawNodes() {
  auto& renderer = *ofGetCurrentRenderer();
  renderer.pushStyle();
  for (const auto& node : _nodes) {
    const auto& pos = node->position();
    renderer.setColor(ofFloatColor(ofFloatColor::goldenRod, 0.8));
    renderer.drawSphere(pos, 8);
    auto radius = node->radius();
    renderer.setColor(ofFloatColor(ofFloatColor::paleGoldenRod, 0.3));
    renderer.drawSphere(pos, radius);
  }
  renderer.popStyle();
}
