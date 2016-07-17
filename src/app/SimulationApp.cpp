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
                                        _context,
                                        _events);
  _observers->setup(_context.state, _appParams.colors);

  _occurrences =
  std::make_shared<OccurrencesController>(_appParams.occurrences,
                                          _appParams.core.bounds,
                                          *_observers,
                                          _context,
                                          _events);
  _occurrences->setup(_appParams.colors);

  _animations =
  std::make_shared<AnimationsController>(_appParams.animations,
                                         _appParams.colors,
                                         _events,
                                         _context);
  _animations->setup();

  _physics = std::make_shared<PhysicsController>(_appParams.physics,
                                                 _appParams.core.bounds,
                                                 _appParams.core.debug,
                                                 _context.state);
  _physics->setup(*_observers, *_occurrences);

  _clock = std::make_shared<Clock>(_appParams.core.clock, _context.state);
  _clock->setup();

  _statusController = std::make_shared<StatusInfoController>();

#ifdef ENABLE_SYPHON
  _syphonServer.setName("Memory Main Output");
#endif
}

void SimulationApp::update() {
  _clock->update();
  _observers->update(_context.state);
  _occurrences->update();
  _animations->update(_context.state);
  _physics->update();
  _renderingController->update(_context.state);
}

void SimulationApp::draw() {
  _renderingController->beginDraw(_context.state);

  _observers->draw(_context.state);
  _occurrences->draw();
  _animations->draw(_context.state);
  _physics->draw();

  if (_appParams.core.debug.showBounds()) {
    ofPushStyle();
    ofNoFill();
    ofSetColor(_appParams.colors.getColor(ColorId::BOUNDS));
    ofDrawBox(_appParams.core.bounds.size());
    ofPopStyle();
  }

  _renderingController->endDraw(_context.state);

#ifdef ENABLE_SYPHON
  if (_appParams.core.output.externalEnabled()) {
    _renderingController->pushToSyphon(_syphonServer);
  }
#endif

  if (_appParams.core.debug.showStatus()) {
    _statusController->draw(_context.state);
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
