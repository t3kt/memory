//
//  Entity.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "Entity.h"
#include <ofMain.h>
#include "Common.h"

static EntityId nextId() {
  static EntityId lastId = 0;
  return ++lastId;
}

Entity::Entity()
: id(nextId()) {
  position = createRandomVec3f(ofVec3f(-1), ofVec3f(1));
}

Entity& Entity::addBehavior(shared_ptr<Behavior> behavior) {
  _behaviors.push_back(behavior);
  return *this;
}

void Entity::update(State &state) {
  velocity = ofVec3f::zero();
  updateBehaviors(_behaviors, *this, state);
  position += velocity;
  position = wrapVec(position, -1, 1);
}

void Entity::draw(State &state) {
  ofPushStyle();
  ofSetColor(color);
  ofDrawSphere(position, radius);
  ofPopStyle();
}
