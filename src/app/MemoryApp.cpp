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
const int START_OCCURRENCES = 10;

static ofVec3f randomPosition() {
  return ofVec3f(ofRandom(-1, 1),
                 ofRandom(-1, 1),
                 ofRandom(-1, 1));
}

void MemoryApp::setup() {
  
  _gui.setup(_appParams.paramGroup);
  
  ofEnableAlphaBlending();
//  ofDisableDepthTest();
  
  _appParams.observer.lifetime.set(10, 60);
  for (int i = 0; i < START_OBSERVERS; i++) {
    spawnObserver();
  }
  
  for (int i = 0; i < START_OCCURRENCES; i++) {
    spawnOccurrence();
  }
  //...
}

void MemoryApp::update() {
  _state.updateTime();
  _observers.update(_state);
  _occurrences.update(_state);
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
  size *= 0.4;
  ofScale(size, size, size);
  
  _observers.draw(_state);
  _occurrences.draw(_state);
  
  ofPopMatrix();
  ofPopStyle();
  _cam.end();
  glPopAttrib();
  
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
  _gui.draw();
}

void MemoryApp::spawnObserver() {
  ofVec3f pos = randomPosition();
  float life = _appParams.observer.lifetime.getValue();
  auto observer = shared_ptr<ObserverEntity>(new ObserverEntity(pos, life, _state));
  _observers.addEntity(observer);
  
  std::cout << "Spawned observer: " << *observer << std::endl;
}

void MemoryApp::spawnOccurrence() {
  ofVec3f pos = randomPosition();
  float radius = _appParams.occurrence.radius.getValue();
  auto occurrence = shared_ptr<OccurrenceEntity>(new OccurrenceEntity(pos, radius));
  
  bool connected = false;
  
  _observers.performAction([&] (shared_ptr<ObserverEntity> observer) {
    float dist = pos.distance(observer->position);
    if (dist <= radius) {
      occurrence->addObserver(observer);
      observer->addOccurrence(occurrence);
      connected = true;
    }
    
  });
  
  if (connected) {
    std::cout << "Spawned occurrence: " << *occurrence << std::endl;
    _occurrences.addEntity(occurrence);
  } else {
    std::cout << "Nothing in range of occurrence: " << *occurrence << std::endl;
  }
  //...
}
