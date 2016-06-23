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
  ofEnableAlphaBlending();
//  ofDisableDepthTest();
  
  _appParams.observer.lifetimeRange.set(ofVec2f(10, 60));
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
//  for (auto observer : _observers) {
//    observer->update(_state);
//  }
  for (auto i = _observers.begin();
       i != _observers.end();) {
    shared_ptr<ObserverEntity>& observer = *i;
    bool dead = false;
    if (!observer) {
      dead = true;
    } else {
      if (observer->getRemainingLifetimeFraction(_state) <= 0) {
        dead = true;
      }
    }
    if (dead) {
      for (auto occurrence : observer->getConnectedOccurrences()) {
        occurrence->removeObserver(observer);
      }
      i = _observers.erase(i);
    } else {
      i++;
    }
  }
  for (auto i = _occurrences.begin();
       i != _occurrences.end(); ) {
    shared_ptr<OccurrenceEntity>& occurrence = *i;
    bool done = false;
    if (!occurrence) {
      done = true;
    } else {
      occurrence->update(_state);
      if (!occurrence->hasConnectedObservers()) {
        done = true;
      }
    }
    if (done) {
      i = _occurrences.erase(i);
    } else {
      i++;
    }
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
  size *= 0.4;
  ofScale(size, size, size);
  //...
  
  for (auto observer : _observers) {
    observer->draw(_state);
  }
  for (auto occurrence : _occurrences) {
    occurrence->draw(_state);
  }
  
  ofPopMatrix();
  ofPopStyle();
  _cam.end();
  glPopAttrib();
  
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
}

void MemoryApp::spawnObserver() {
  ofVec3f pos = randomPosition();
  float life = ofRandom(_appParams.observer.lifetimeRange.get()[0], _appParams.observer.lifetimeRange.get()[1]);
  auto observer = shared_ptr<ObserverEntity>(new ObserverEntity(pos, life, _state));
  _observers.push_back(observer);
  
  std::cout << "Spawned observer: " << *observer << std::endl;
}

void MemoryApp::spawnOccurrence() {
  ofVec3f pos = randomPosition();
  float radius = ofRandom(_appParams.occurrence.radiusRange.get()[0], _appParams.occurrence.radiusRange.get()[1]);
  auto occurrence = shared_ptr<OccurrenceEntity>(new OccurrenceEntity(pos, radius));
  
  bool connected = false;
  
  for (auto observer : _observers) {
    float dist = pos.distance(observer->position);
    if (dist <= radius) {
      occurrence->addObserver(observer);
      observer->addOccurrence(occurrence);
      connected = true;
    }
  }
  
  if (connected) {
    std::cout << "Spawned occurrence: " << *occurrence << std::endl;
    _occurrences.push_back(occurrence);
  } else {
    std::cout << "Nothing in range of occurrence: " << *occurrence << std::endl;
  }
  //...
}
