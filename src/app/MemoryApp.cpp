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
#include <ofxGuiExtended.h>

class FPSInfoProvider
: public StatusInfoProvider {
public:
  void setup() {
    FPS_LINE = _status.registerLine("FPS:");
  }
  void update(const State& state) {
    _status.setValue(FPS_LINE, ofToString(ofGetFrameRate(), 2));
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

  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams);
  _gui->setup();
  _gui->onLoad += [&]() {
    loadSettings();
  };
  _gui->onSave += [&]() {
    saveSettings();
  };

  ofEnableAlphaBlending();
  ofDisableDepthTest();
  _postProc.init();
  _postProc.createPass<BloomPass>()->setEnabled(true);
  
  _observers = std::make_shared<ObserversController>(_appParams.observers, _appParams.core.bounds, _state, _appParams.colors);
  _observers->setup(_state);
  
  _occurrences = std::make_shared<OccurrencesController>(_appParams.occurrences, _appParams.core.bounds, *_observers, _state, _appParams.colors);
  _occurrences->setup(_state);
  
  _animations = std::make_shared<AnimationsController>(_appParams.animations, _appParams.colors);
  _animations->setup();
  _animations->attachTo(*_observers);
  _animations->attachTo(*_occurrences);

  _clock = std::make_shared<Clock>(_appParams.core.clock, _state);
  _clock->setup();

  _fpsProvider = std::make_shared<FPSInfoProvider>();
  _fpsProvider->setup();

  _statusController = std::make_shared<StatusInfoController>();
  _statusController->setup();
  _statusController->addProvider(_clock.get());
  _statusController->addProvider(_fpsProvider.get());
  _statusController->addProvider(_observers.get());
  _statusController->addProvider(_occurrences.get());
  _statusController->addProvider(_animations.get());

#ifdef ENABLE_SYPHON
  _syphonServer.setName("Memory Main Output");
#endif
}

void MemoryApp::update() {
  _clock->update();
  _observers->update(_state);
  _occurrences->update(_state);
  _animations->update(_state);

  // Slide the logger screen in and out.
  ofRectangle bounds = _screenLoggerChannel->getDrawBounds();
  if (_appParams.core.debug.showLog()) {
    bounds.y = ofLerp(bounds.y, 0, 0.2);
  }
  else {
    bounds.y = ofLerp(bounds.y, -bounds.height, 0.2);
  }
  _screenLoggerChannel->setDrawBounds(bounds);
  _fpsProvider->update(_state);
  _gui->update();
}

void MemoryApp::draw() {
  ofBackground(_appParams.colors.getColor(ColorId::BACKGROUND));
  glPushAttrib(GL_ENABLE_BIT);
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  _postProc.begin(_cam);
  
  ofPushMatrix();
  ofPushStyle();

  if (_appParams.core.camera.spinEnabled()) {
    ofVec3f dr = _appParams.core.camera.spinRate() * _state.timeDelta;
    _rotation += dr;
  }
  ofRotateX(_rotation.x);
  ofRotateY(_rotation.y);
  ofRotateZ(_rotation.z);
  
  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  size *= 0.4;
  ofScale(size, size, size);
  
  _observers->draw(_state);
  _occurrences->draw(_state);
  _animations->draw(_state);

  if (_appParams.core.debug.showBounds()) {
    ofPushStyle();
    ofNoFill();
    ofSetColor(_appParams.colors.getColor(ColorId::BOUNDS));
    ofDrawBox(_appParams.core.bounds.size());
    ofPopStyle();
  }
  
  ofPopMatrix();
  ofPopStyle();
  _postProc.end();
  glPopAttrib();

#ifdef ENABLE_SYPHON
  if (_appParams.core.syphonEnabled()) {
    _syphonServer.publishScreen();
  }
#endif
  
  _gui->draw();
  _screenLoggerChannel->draw();

  if (_appParams.core.debug.showStatus()) {
    _statusController->draw();
  }
}

void MemoryApp::keyPressed(int key) {
  switch (key) {
    case 'h':
      _cam.reset();
      break;
    case 'l':
      _appParams.core.debug.setShowLog(!_appParams.core.debug.showLog());
      break;
    case ' ':
      _clock->toggleState();
      break;
    case '9':
      _occurrences->spawnOccurrences(5, _state);
      break;
    case '0':
      _observers->spawnObservers(5, _state);
      break;
    case 'r':
      loadSettings();
      break;
    case 'w':
      saveSettings();
      break;
  }
}

void MemoryApp::loadSettings() {
  ofLogNotice() << "Reading JSON settings...";
  _appParams.readFromFile("settings.json");
  ofLogNotice() << ".. read from JSON finished\n\t" << _appParams;
}

void MemoryApp::saveSettings() {
  ofLogNotice() << "Writing JSON settings...";
  _appParams.writeToFile("settings.json");
  ofLogNotice() << ".. write to JSON finished";
}
