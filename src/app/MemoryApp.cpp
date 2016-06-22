//
//  MemoryApp.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#include "MemoryApp.h"
#include "Common.h"

const int START_OBSERVERS = 20;

void MemoryApp::setup() {
  ofEnableAlphaBlending();
  
  _appParams.observer.lifetimeRange.set(ofVec2f(5000, 60000));
  //...
}

void MemoryApp::update() {
  _state.updateTime();
  //...
}

void MemoryApp::draw() {
  ofClear(0);
  ofBackground(ofColor::white);
  
  ofPushMatrix();
  ofPushStyle();
  
  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  ofScale(size, size, size);
  //...
  
  ofPopMatrix();
  ofPopStyle();
  
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
}

void MemoryApp::spawnObserver() {
  ofVec3f pos = createRandomVec3f(ofVec3f::zero(), ofVec3f(ofGetWidth(), ofGetHeight(), 0));
  float life = ofRandom(_appParams.observer.lifetimeRange.get()[0], _appParams.observer.lifetimeRange.get()[1]);
  _observers.push_back(shared_ptr<ObserverEntity>(new ObserverEntity(pos, life, _state)));
}

void MemoryApp::spawnOccurrence() {
  //...
}
