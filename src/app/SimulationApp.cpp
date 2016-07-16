//
//  SimulationApp.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include "AppSystem.h"
#include "SimulationApp.h"

void SimulationApp::setup() {
  _renderingController =
  std::make_shared<RenderingController>(_appParams.rendering,
                                        getWindow(),
                                        _appParams.colors);
  _renderingController->setup();

  _appParams.core.output.fullscreen.changed += [&](bool& fullscreen) {
    _window->setFullscreen(fullscreen);
    _renderingController->updateResolution();
  };

  registerAsActionHandler();

  _observers =
  std::make_shared<ObserversController>(_appParams.observers,
                                        _appParams.core.bounds,
                                        _state,
                                        _events);
  _observers->setup(_state, _appParams.colors);

  _occurrences =
  std::make_shared<OccurrencesController>(_appParams.occurrences,
                                          _appParams.core.bounds,
                                          *_observers,
                                          _state,
                                          _events);
  _occurrences->setup(_state, _appParams.colors);

  _animations =
  std::make_shared<AnimationsController>(_appParams.animations,
                                         _appParams.colors,
                                         _events);
  _animations->setup();

  _physics = std::make_shared<PhysicsController>(_appParams.physics,
                                                 _appParams.core.bounds,
                                                 _appParams.core.debug,
                                                 _state);
  _physics->setup(*_observers, *_occurrences);

  _clock = std::make_shared<Clock>(_appParams.core.clock, _state);
  _clock->setup();

  _statusController = std::make_shared<StatusInfoController>();

#ifdef ENABLE_SYPHON
  _syphonServer.setName("Memory Main Output");
#endif
}

void SimulationApp::update() {
  _clock->update();
  _observers->update(_state);
  _occurrences->update(_state);
  _animations->update(_state);
  _physics->update();
  _renderingController->update(_state);
}

void SimulationApp::draw() {
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
  if (_appParams.core.output.externalEnabled()) {
    _renderingController->pushToSyphon(_syphonServer);
  }
#endif

  if (_appParams.core.debug.showStatus()) {
    _statusController->draw(_state);
  }
}

void SimulationApp::keyPressed(ofKeyEventArgs& event) {
  AppSystem::get().handleKeyPressed(event);
}

bool SimulationApp::performAction(AppAction action) {
  switch (action) {
    default:
      return false;
  }
  return true;
}
