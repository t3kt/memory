//
//  CameraController.cpp
//

#include <ofMain.h>
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
#include "../rendering/CameraController.h"

void CameraController::setup() {
  AppSystem::get().commands()
  .registerCommand("resetCamera", "Reset Camera", [&](const CommandArgs&) {
    resetCamera();
    return true;
  }, true, 'h');
}

void CameraController::resetCamera() {
  _cam.reset();
}

void CameraController::update() {
  if (_params.spinEnabled) {
    _rotation += _params.spinRate.get() * _context.rootState.timeDelta;
  }
}

void CameraController::applyTransform() {
  ofRotateXDeg(_rotation.x);
  ofRotateYDeg(_rotation.y);
  ofRotateZDeg(_rotation.z);
}
