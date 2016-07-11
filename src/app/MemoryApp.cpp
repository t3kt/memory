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

void MemoryApp::setup() {
  ofSetLogLevel(_appParams.core.debug.loggingEnabled()
                ? OF_LOG_NOTICE : OF_LOG_ERROR);
  loadSettings();

  _gui = std::make_shared<AppGui>(_appParams);
  _gui->setup();
  _gui->onLoad += [&]() {
    loadSettings();
  };
  _gui->onSave += [&]() {
    saveSettings();
  };

  _renderingController = RenderingController::create(_appParams.rendering, _appParams.colors);
  
  _observers = std::make_shared<ObserversController>(_appParams.observers, _appParams.core.bounds, _state);
  _observers->setup(_state, _appParams.colors);
  
  _occurrences = std::make_shared<OccurrencesController>(_appParams.occurrences, _appParams.core.bounds, *_observers, _state);
  _occurrences->setup(_state, _appParams.colors);
  
  _animations = std::make_shared<AnimationsController>(_appParams.animations, _appParams.colors);
  _animations->setup();
  _animations->attachTo(*_observers);
  _animations->attachTo(*_occurrences);

  _physics = std::make_shared<PhysicsController>(_appParams.physics,
                                                 _appParams.core.bounds,
                                                 _appParams.core.debug,
                                                 _state);
  _physics->setup(*_observers, *_occurrences);

  _clock = std::make_shared<Clock>(_appParams.core.clock, _state);
  _clock->setup();

  _statusController = std::make_shared<StatusInfoController>(*_clock,
                                                             *_observers,
                                                             *_occurrences,
                                                             *_animations);

#ifdef ENABLE_SYPHON
  _syphonServer.setName("Memory Main Output");
#endif
}

void MemoryApp::update() {
  ofSetLogLevel(_appParams.core.debug.loggingEnabled()
                ? OF_LOG_NOTICE : OF_LOG_ERROR);
  _clock->update();
  _observers->update(_state);
  _occurrences->update(_state);
  _animations->update(_state);
  _physics->update();
  _renderingController->update(_state);
  _gui->update();
}

void MemoryApp::draw() {
  _renderingController->beginDraw(_state);
  
  _observers->draw(_state);
  _occurrences->draw(_state);
  _animations->draw(_state);
  _physics->draw();

  if (_appParams.core.debug.showBounds()) {
    ofPushStyle();
    ofNoFill();
    ofSetColor(_appParams.colors.getColor(ColorId::BOUNDS));
    ofDrawBox(_appParams.core.bounds.size());
    ofPopStyle();
  }

  _renderingController->endDraw(_state);

#ifdef ENABLE_SYPHON
  if (_appParams.core.syphonEnabled()) {
    _syphonServer.publishScreen();
  }
#endif
  
  _gui->draw();

  if (_appParams.core.debug.showStatus()) {
    _statusController->draw();
  }
}

void MemoryApp::keyPressed(int key) {
  switch (key) {
    case 'h':
      _renderingController->resetCamera();
      break;
    case 'l':
      _appParams.core.debug.setLoggingEnabled(!_appParams.core.debug.loggingEnabled());
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
    case '(':
      _occurrences->spawnOccurrences(100, _state);
      break;
    case ')':
      _observers->spawnObservers(100, _state);
      break;
    case '-':
      _observers->killObservers(5);
      break;
    case '_':
      _observers->killObservers(100);
      break;
    case 'r':
      loadSettings();
      break;
    case 'w':
      saveSettings();
      break;
    case 'x':
      _physics->stopAllEntities();
      break;
    case 'c':
      ofLogNotice() << "Dumping config JSON:\n"
        << prettyPrintJsonToString(_appParams.to_json());
      break;
    case 'p':
      _appParams.core.debug.setShowPhysics(!_appParams.core.debug.showPhysics());
      break;
    case 'b':
      _appParams.core.debug.setShowBounds(!_appParams.core.debug.showBounds());
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
