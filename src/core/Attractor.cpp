//
//  Attractor.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "Attractor.h"
#include <ofMain.h>

void Attractor::draw(State &state) {
  ofPushStyle();
  ofSetColor(color);
  ofDrawSphere(position, range);
  ofPopStyle();
}

void Attractor::applyTo(Entity *entity) const {
  auto diff = position - entity->position;
  auto dist = diff.length();
  if (dist < range) {
//    auto amount = ofMap(dist, 0, 1, 0, pull);
    auto amount = pull;
    auto force = diff * amount;
    entity->velocity += force;
  }
}
