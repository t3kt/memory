//
//  MemoryApp.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "MemoryApp.h"
#include "Common.h"
#include <memory>
#include <iostream>

const int START_OBSERVERS = 20;

void MemoryApp::setup() {
  ofEnableAlphaBlending();
  
  _appParams.observer.lifetimeRange.set(ofVec2f(10, 60));
  for (int i = 0; i < START_OBSERVERS; i++) {
    spawnObserver();
  }
  //...
}

void MemoryApp::update() {
  _state.updateTime();
  for (auto observer : _observers) {
    observer->update(_state);
  }
  //...
}

void MemoryApp::draw() {
//  ofClear(ofColor::white);
  ofBackground(ofColor::white);
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  _cam.begin();
  
  ofPushMatrix();
  ofPushStyle();
  
  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  size *= 0.7;
  ofScale(size, size, size);
  //...
  
  for (auto observer : _observers) {
    observer->draw(_state);
  }
  
  ofPopMatrix();
  ofPopStyle();
  _cam.end();
  glPopAttrib();
  
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
}

void MemoryApp::spawnObserver() {
  ofVec3f pos;
//  pos.x = ofRandom(0, ofGetWidth());
//  pos.y = ofRandom(0, ofGetHeight());
  pos.x = ofRandom(-1, 1);
  pos.y = ofRandom(-1, 1);
  pos.z = ofRandom(-1, 1);
  //  ofVec3f pos = createRandomVec3f(ofVec3f::zero(), ofVec3f(ofGetWidth(), ofGetHeight(), 0));
  float life = ofRandom(_appParams.observer.lifetimeRange.get()[0], _appParams.observer.lifetimeRange.get()[1]);
  auto observer = shared_ptr<ObserverEntity>(new ObserverEntity(pos, life, _state));
  _observers.push_back(observer);
  
  std::cout << "Spawned observer: " << *observer << std::endl;
}

void MemoryApp::spawnOccurrence() {
  //...
}
