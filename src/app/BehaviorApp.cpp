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
#include "AttractionBehavior.h"

void BehaviorApp::setup() {
  ofEnableAlphaBlending();
  for (int i = 0; i < 1000; i++) {
    ofPtr<Entity> entity(new Entity());
    entity->color = ofFloatColor(0, 0.2, 1);
    entity->radius = 0.005;
    entity->addBehavior<RandomWalkBehavior>()
      .setSpeed(i % 5 == 0 ? 0.004 : 0.002);
    if (i % 3 == 0) {
      entity->addBehavior<AttractionBehavior>()
        .setRange(0.004, 0.2)
        .setPull(0.002, 0.0001)
        .setLimit(10);
      entity->color = ofFloatColor(0.7, 0.1, 0);
    } else if (i % 3 == 1) {
      entity->addBehavior<AttractionBehavior>()
        .setRange(0.004, 0.2)
        .setPull(-0.002, -0.0001)
        .setLimit(10);
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
