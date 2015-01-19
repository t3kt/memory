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
: id(nextId()) { }

void Entity::update(State &state) {
  position += velocity;
  position = wrapVec(position, -1, 1);
}

void Entity::draw(State &state) {
  ofPushStyle();
//  ofPushMatrix();
  ofSetColor(color);
//  ofTranslate(position);
//  ofDrawSphere(5);
  ofDrawSphere(position, .01);
//  ofPopMatrix();
  ofPopStyle();
}
