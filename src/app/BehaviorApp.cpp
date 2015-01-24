//
//  BehaviorApp.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "BehaviorApp.h"
#include "Common.h"
#include "RandomWalkBehavior.h"
#include "AttractionBehavior.h"

static shared_ptr<Behavior> createPullBehavior() {
  auto behavior = new EntityAttractionBehavior();
  behavior->setRange(0.016, 0.4)
    .setPull(-0.0002, 0.002)
    .setLimit(10);
  return shared_ptr<Behavior>(behavior);
}

static shared_ptr<Behavior> createPushBehavior() {
  auto behavior = new EntityAttractionBehavior();
  behavior->setRange(0.008, 0.4)
    .setPull(-0.005, -0.0001)
    .setLimit(10);
  return shared_ptr<Behavior>(behavior);
}

static shared_ptr<Behavior> createCenterAttractor() {
  auto behavior = new AttractionBehavior();
  behavior->setRange(0.5, 3)
    .setPull(0, 0.5)
    .setPosition(ofVec3f(0, 0, 0));
  return shared_ptr<Behavior>(behavior);
}

void BehaviorApp::setup() {
  ofEnableAlphaBlending();
  auto centerAttractor = createCenterAttractor();
  auto pullBehavior = createPullBehavior();
  auto pushBehavior = createPushBehavior();
  for (int i = 0; i < 1000; i++) {
    ofPtr<Entity> entity(new Entity());
    entity->position = createRandomVec3f(ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
    entity->color = ofFloatColor(0, 0.2, 1);
    entity->radius = 0.005;
    entity->addBehavior(centerAttractor);
    entity->addBehavior<RandomWalkBehavior>()
      .setSpeed(i % 5 == 0 ? 0.004 : 0.002);
    if (i % 3 == 0) {
      entity->addBehavior(pullBehavior);
      entity->color = ofFloatColor(0.7, 0.1, 0);
    } else if (i % 6 == 1) {
      entity->addBehavior(pushBehavior);
      entity->color = ofFloatColor(0.1, 0.7, 0);
    }
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
  glPopAttrib();
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), 20, 20);
}
