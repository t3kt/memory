//
//  BehaviorApp.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "BehaviorApp.h"
#include "Common.h"
#include "Attractor.h"
#include "RandomWalkBehavior.h"

void BehaviorApp::setup() {
  ofEnableAlphaBlending();
  for (int i = 0; i < 50; i++) {
    ofPtr<Entity> entity(new Entity());
    entity->color = ofFloatColor(0, 0.2, 1);
    entity->addBehavior(shared_ptr<Behavior>(new RandomWalkBehavior()));
    _state.entities.push_back(entity);
  }
}

void BehaviorApp::update() {
  _state.updateTime();
  for (auto& entity : _state.entities) {
    entity->update(_state);
  }
}

void BehaviorApp::draw() {
  ofClear(0);
  ofBackground(0);
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  _cam.begin();
  ofPushMatrix();
  ofPushStyle();
  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  ofScale(size, size, size);
  for (auto& entity : _state.entities) {
    entity->draw(_state);
  }
  ofPopStyle();
  ofPopMatrix();
  _cam.end();
}
