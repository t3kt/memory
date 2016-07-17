//
//  CameraController.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include <ofMain.h>
#include "CameraController.h"

void CameraController::setup() {
  registerAsActionHandler();
}

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

void CameraController::update(const State& state) {
  if (_params.spinEnabled) {
    _rotation += _params.spinRate.get() * state.timeDelta;
  }
}

void CameraController::applyTransform() {
  ofRotateX(_rotation.x);
  ofRotateY(_rotation.y);
  ofRotateZ(_rotation.z);
}
