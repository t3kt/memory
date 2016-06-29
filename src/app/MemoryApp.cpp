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

class FPSInfoProvider
: public StatusInfoProvider {
public:
  void setup() {
    FPS_LINE = _status.registerLine("FPS:");
  }
  void update(const State& state) {
    _status.setValue(FPS_LINE, ofToString(ofGetFrameRate()));
  }
  const StatusInfo& getStatusInfo() const { return _status; }
private:
  StatusInfo _status;
  std::size_t FPS_LINE;
};

void MemoryApp::setup() {
  _screenLoggerChannel = std::make_shared<ofxScreenLoggerChannel>();
  _screenLoggerChannel->setDrawBounds(ofRectangle(150, 0, ofGetScreenWidth() - 500, 250));
  _multiLoggerChannel = std::make_shared<ofxMultiLoggerChannel>();
  _multiLoggerChannel->addConsoleLoggerChannel();
  _multiLoggerChannel->addLoggerChannel(_screenLoggerChannel);
  ofSetLoggerChannel(_multiLoggerChannel);
  
  _appParams.initGui(_gui);
  
  ofEnableAlphaBlending();
  ofDisableDepthTest();

  _appParams.observers.entities.lifetime.set(10, 60);
  
  _observers = std::make_shared<ObserversController>(_appParams.observers, _appParams.bounds, _state);
  _observers->setup(_state);
  
  _occurrences = std::make_shared<OccurrencesController>(_appParams.occurrences, _appParams.bounds, *_observers, _state);
  _occurrences->setup(_state);
  
  _animations = std::make_shared<AnimationsController>(_appParams.animations);
  _animations->setup();
  _animations->attachTo(*_observers);
  _animations->attachTo(*_occurrences);

  _fpsProvider = std::make_shared<FPSInfoProvider>();
  _fpsProvider->setup();

  _statusController = std::make_shared<StatusInfoController>();
  _statusController->setup();
  _statusController->addProvider(_fpsProvider.get());
  _statusController->addProvider(_observers.get());
  _statusController->addProvider(_occurrences.get());
  _statusController->addProvider(_animations.get());

//  _NEW_gui = std::make_shared<AppGui>(_appParams);
//  _NEW_gui->setup();
  //...
}

void MemoryApp::update() {
  _state.updateTime();
  _observers->update(_state);
  _occurrences->update(_state);
  _animations->update(_state);

  // Slide the logger screen in and out.
  ofRectangle bounds = _screenLoggerChannel->getDrawBounds();
  if (_appParams.debug.showLog.get()) {
    bounds.y = ofLerp(bounds.y, 0, 0.2);
  }
  else {
    bounds.y = ofLerp(bounds.y, -bounds.height, 0.2);
  }
  _screenLoggerChannel->setDrawBounds(bounds);
  _fpsProvider->update(_state);
//  _NEW_gui->update();
}

void MemoryApp::draw() {
  ofBackground(ofFloatColor::white);
  glPushAttrib(GL_ENABLE_BIT);
  //glEnable(GL_DEPTH_TEST);
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

  if (_appParams.debug.showBounds.get()) {
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofFloatColor(0.2, 0.2, 0.2, 0.3));
    ofDrawBox(_appParams.bounds.size.get());
    ofPopStyle();
  }
  
  ofPopMatrix();
  ofPopStyle();
  _cam.end();
  glPopAttrib();
  
  _gui.draw();
  _screenLoggerChannel->draw();

  if (_appParams.debug.showStatus.get()) {
    _statusController->draw();
  }
//  _NEW_gui->draw();
}

void MemoryApp::keyPressed(int key) {
  if (key == 'h') {
    _cam.reset();
  } else if (key == 'l') {
    _appParams.debug.showLog.set(!_appParams.debug.showLog.get());
  }
}
