//
//  CameraController.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include <ofMain.h>
#include "../rendering/CameraController.h"

void CameraController::resetCamera() {
  _cam.reset();
}

bool CameraController::performAction(AppAction action) {
  switch (action) {
    case AppAction::RESET_CAMERA:
      resetCamera();
      break;
    default:
      return false;
  }
  return true;
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
