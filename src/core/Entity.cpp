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
  noisePos = createRandomVec3f(10000000);
  position = createRandomVec3f(ofVec3f(-1), ofVec3f(1));
}

void Entity::update(State &state) {
  position += velocity;
  position = wrapVec(position, -1, 1);
}

void Entity::draw(State &state) {
  ofPushStyle();
  ofSetColor(color);
  ofDrawSphere(position, .01);
  ofPopStyle();
}
