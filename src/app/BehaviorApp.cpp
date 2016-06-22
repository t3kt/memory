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

static shared_ptr<Behavior> makeBehavior(const SingleAttractionBehavior::Params& params) {
  return shared_ptr<Behavior>(new SingleAttractionBehavior(params));
}

static shared_ptr<Behavior> makeBehavior(const EntityAttractionBehavior::Params& params) {
  return shared_ptr<Behavior>(new EntityAttractionBehavior(params));
}

static shared_ptr<Behavior> makeBehavior(const RandomWalkBehavior::Params& params) {
  return shared_ptr<Behavior>(new RandomWalkBehavior(params));
}

void BehaviorApp::setup() {
  ofEnableAlphaBlending();
  _appParams.centerAttraction
    .setRange(0.5, 3)
    .setPull(-0.1, 0.5)
    .setPosition(ofVec3f(0, 0, 0));
  _appParams.entityAttraction
    .setRange(0.016, 0.4)
    .setPull(-0.0002, 0.002)
    .setLimit(10);
  _appParams.attractorWalk
    .setSpeed(0.01);
  _appParams.entityRepulsion
    .setRange(0.008, 0.4)
    .setPull(-0.005, -0.0001)
    .setLimit(10);
  _appParams.repulsorWalk
    .setSpeed(0.004);
  _appParams.otherWalk
    .setSpeed(0.002);
  auto centerAttractor = makeBehavior(_appParams.centerAttraction);
  auto pullBehavior = makeBehavior(_appParams.entityAttraction);
  auto pushBehavior = makeBehavior(_appParams.entityRepulsion);
  auto attractorWalk = makeBehavior(_appParams.attractorWalk);
  auto repulsorWalk = makeBehavior(_appParams.repulsorWalk);
  auto otherWalk = makeBehavior(_appParams.otherWalk);
  for (int i = 0; i < 8000; i++) {
    ofPtr<Entity> entity(new Entity());
    entity->position = createRandomVec3f(ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
    entity->color = ofFloatColor(0, 0.2, 1);
    entity->drawRadius = 0.003;
    entity->addBehavior(centerAttractor);
    if (i % 3 == 0) {
      entity->addBehavior(pullBehavior);
      entity->color = ofFloatColor(0.7, 0.1, 0);
      entity->addBehavior(attractorWalk);
    } else if (i % 6 == 1) {
      entity->addBehavior(pushBehavior);
      entity->color = ofFloatColor(0.1, 0.7, 0);
      entity->addBehavior(repulsorWalk);
    } else {
      entity->addBehavior(otherWalk);
    }
    _mesh.addVertex(entity->position);
    _mesh.addColor(entity->color);
    _state.entities.push_back(entity);
  }
  _gui.setup(_appParams.paramGroup);
}

void BehaviorApp::update() {
  _state.updateTime();
  int i = 0;
  for (auto& entity : _state.entities) {
    entity->update(_state);
    _mesh.setVertex(i, entity->position);
    i++;
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
//  for (auto& entity : _state.entities) {
//    entity->draw(_state);
//  }
  _mesh.draw(OF_MESH_POINTS);
  ofPopStyle();
  ofPopMatrix();
  _cam.end();
  glPopAttrib();
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
  _gui.draw();
}
