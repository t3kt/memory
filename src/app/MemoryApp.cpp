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
    _status.setValue(FPS_LINE, ofToString(ofGetFrameRate(), 2));
  }
  const StatusInfo& getStatusInfo() const { return _status; }
private:
  StatusInfo _status;
  std::size_t FPS_LINE;
};

class GuiPanel
: public ofxPanel {
public:
  GuiPanel() {
  }

  void setup(MemoryAppParameters& appParams) {
    ofxPanel::setup();
    setName("Memory Controls");
    actionsGroup.setName("Actions");
    paramsGroup.setName("Parameters");
    actionsGroup.setup();
    loadButton.setup("load");
    saveButton.setup("save");
    loadButton.addListener(this, &GuiPanel::onLoadClick);
    saveButton.addListener(this, &GuiPanel::onSaveClick);
    actionsGroup.add(&loadButton);
    actionsGroup.add(&saveButton);
    paramsGroup.setup(appParams);
    add(&actionsGroup);
    add(&paramsGroup);
  }

  ofxLiquidEvent<void> onLoad;
  ofxLiquidEvent<void> onSave;

private:
  void onLoadClick(void) {
    onLoad.notifyListeners();
  }
  void onSaveClick(void) {
    onSave.notifyListeners();
  }

  ofxGuiGroup actionsGroup;
  ofxButton loadButton;
  ofxButton saveButton;
  ofxGuiGroup paramsGroup;
};

void MemoryApp::setup() {
  _screenLoggerChannel = std::make_shared<ofxScreenLoggerChannel>();
  _screenLoggerChannel->setDrawBounds(ofRectangle(150, 0, ofGetScreenWidth() - 500, 250));
  _multiLoggerChannel = std::make_shared<ofxMultiLoggerChannel>();
  _multiLoggerChannel->addConsoleLoggerChannel();
  _multiLoggerChannel->addLoggerChannel(_screenLoggerChannel);
  ofSetLoggerChannel(_multiLoggerChannel);

  _appParams.observers.entities.lifetime.setParamValues(10, 60);  

  _gui = std::make_shared<GuiPanel>();
  _gui->setup(_appParams);
  _gui->onLoad += [&]() {
    loadSettings();
  };
  _gui->onSave += [&]() {
    saveSettings();
  };

  ofEnableAlphaBlending();
  ofDisableDepthTest();
  
  _observers = std::make_shared<ObserversController>(_appParams.observers, _appParams.bounds, _state, _appParams.colors);
  _observers->setup(_state);
  
  _occurrences = std::make_shared<OccurrencesController>(_appParams.occurrences, _appParams.bounds, *_observers, _state, _appParams.colors);
  _occurrences->setup(_state);
  
  _animations = std::make_shared<AnimationsController>(_appParams.animations, _appParams.colors);
  _animations->setup();
  _animations->attachTo(*_observers);
  _animations->attachTo(*_occurrences);

  _clock = std::make_shared<Clock>(_appParams.clock, _state);
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

//  _NEW_gui = std::make_shared<AppGui>(_appParams);
//  _NEW_gui->setup();
  //...
}

void MemoryApp::update() {
  _clock->update();
  _observers->update(_state);
  _occurrences->update(_state);
  _animations->update(_state);

  // Slide the logger screen in and out.
  ofRectangle bounds = _screenLoggerChannel->getDrawBounds();
  if (_appParams.debug.showLog()) {
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
  ofBackground(_appParams.colors.background.get());
  glPushAttrib(GL_ENABLE_BIT);
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  _cam.begin();
  
  ofPushMatrix();
  ofPushStyle();

  if (_appParams.camera.spinEnabled()) {
    ofVec3f dr = _appParams.camera.spinRate() * _state.timeDelta;
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

  if (_appParams.debug.showBounds()) {
    ofPushStyle();
    ofNoFill();
    ofSetColor(_appParams.colors.bounds.get());
    ofDrawBox(_appParams.bounds.size.get());
    ofPopStyle();
  }
  
  ofPopMatrix();
  ofPopStyle();
  _cam.end();
  glPopAttrib();

#ifdef ENABLE_SYPHON
  _syphonServer.publishScreen();
#endif
  
  _gui->draw();
  _screenLoggerChannel->draw();

  if (_appParams.debug.showStatus()) {
    _statusController->draw();
  }
//  _NEW_gui->draw();
}

void MemoryApp::keyPressed(int key) {
  switch (key) {
    case 'h':
      _cam.reset();
      break;
    case 'l':
      _appParams.debug.setShowLog(!_appParams.debug.showLog());
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
    case 'j':
      std::cout << "PARAMS JSON:\n" << _appParams.to_json().dump() << std::endl;
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
  std::cout << "Reading JSON settings..." << std::endl;
  _appParams.readFromFile("settings.json");
  std::cout << ".. read from JSON finished" << std::endl;
  std::cout << _appParams << std::endl;
}

void MemoryApp::saveSettings() {
  std::cout << "Writing JSON settings..." << std::endl;
  _appParams.writeToFile("settings.json");
  std::cout << ".. write to JSON finished" << std::endl;
}
