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

const int START_OCCURRENCES = 10;

static ofVec3f randomPosition() {
  return ofVec3f(ofRandom(-1, 1),
                 ofRandom(-1, 1),
                 ofRandom(-1, 1));
}

void MemoryApp::setup() {
  
  _appParams.initGui(_gui);
  
  ofEnableAlphaBlending();
//  ofDisableDepthTest();
  
  _appParams.observers.entities.lifetime.set(10, 60);
  
  _observers = std::make_shared<ObserversController>(_appParams.observers, _state);
  _observers->setup(_state);
  
  _occurrences = std::make_shared<OccurrencesController>(_appParams.occurrences, *_observers, _state);
  _occurrences->setup(_state);
  
  _animations = std::make_shared<AnimationsController>(_appParams.animations);
  _animations->attachTo(*_observers);
  _animations->attachTo(*_occurrences);
  //...
}

void MemoryApp::update() {
  _state.updateTime();
  _observers->update(_state);
  _occurrences->update(_state);
  _animations->update(_state);
}

void MemoryApp::draw() {
  ofBackground(ofFloatColor::white);
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
  
  _observers->draw(_state);
  _occurrences->draw(_state);
  _animations->draw(_state);
  
  ofPopMatrix();
  ofPopStyle();
  _cam.end();
  glPopAttrib();
  
  ofSetColor(255);
  ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 140, 20);
  ofDrawBitmapStringHighlight("Observers: " + ofToString(_observers->count()), ofGetWidth() - 140, 40);
  ofDrawBitmapStringHighlight("Occurrences: " + ofToString(_occurrences->count()), ofGetWidth() - 140, 60);
  _gui.draw();
}

void MemoryApp::keyPressed(int key) {
  if (key == 'h') {
    _cam.reset();
  }
}
