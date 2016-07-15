//
//  RenderingController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "RenderingController.h"
#include <ofMain.h>

RenderingController::RenderingController(const Params& params,
                                         ofAppGLFWWindow& window,
                                         const ColorTheme& colors)
: _params(params)
, _window(window)
, _colors(colors)
, _backgroundColor(colors.getColor(ColorId::BACKGROUND))
, _fogColor(colors.getColor(ColorId::FOG)) {
}

void RenderingController::setup() {
  ofEnableAlphaBlending();
  _postProc = std::make_shared<PostProcController>(_params.postProc);
  _postProc->setup();
  //  _light.setDirectional();
  //  _light.setPosition(ofVec3f(0, 3, 0));
  //  _light.setDiffuseColor(ofFloatColor::red);
  //  _light.setAttenuation(4);
  registerWithAppSystem();
}

void RenderingController::updateResolution() {
  auto size = ofVec2f(_window.getWidth(), _window.getHeight());
  _postProc->updateResolution(size);
}

void RenderingController::resetCamera() {
  _cam.reset();
}

bool RenderingController::performAction(AppAction action) {
  switch (action) {
    case AppAction::RESET_CAMERA:
      resetCamera();
      break;
    default:
      return false;
  }
  return true;
}

void RenderingController::update(const State &state) {
  if (_params.camera.spinEnabled()) {
    _rotation += _params.camera.spinRate() * state.timeDelta;
  }
  _postProc->update(state);
}

void RenderingController::beginDraw(const State &state) {
  ofBackground(_backgroundColor);
  glPushAttrib(GL_ENABLE_BIT);
//  ofEnableDepthTest();
  //glEnable(GL_DEPTH_TEST);
//  ofEnableLighting();
  glEnable(GL_CULL_FACE);
//  _light.enable();
  _postProc->beginDraw(_cam);
  if (_params.fog.enabled()) {
    beginFog();
  }

  ofPushMatrix();
  ofRotateX(_rotation.x);
  ofRotateY(_rotation.y);
  ofRotateZ(_rotation.z);

  auto winSize = ofGetWindowSize();
  auto size = ::min(winSize.x, winSize.y) / 2;
  size *= 0.4;
  ofScale(size, size, size);
}

void RenderingController::endDraw(const State &state) {
  ofPopMatrix();
  if (_params.fog.enabled()) {
    endFog();
  }
  _postProc->endDraw(_cam);
//  ofDisableDepthTest();
//  ofDisableLighting();
  glPopAttrib();
}

void RenderingController::beginFog() {
  GLfloat fogCol[4];
  if (_params.fog.useBackgroundColor()) {
    fogCol[0] = _backgroundColor.r;
    fogCol[1] = _backgroundColor.g;
    fogCol[2] = _backgroundColor.b;
  } else {
    fogCol[0] = _fogColor.r;
    fogCol[1] = _fogColor.g;
    fogCol[2] = _fogColor.b;
  }
  fogCol[3] = 1; // not used by opengl?

  glFogi(GL_FOG_MODE, GL_EXP2);
  glFogfv(GL_FOG_COLOR, fogCol);
  glFogf(GL_FOG_DENSITY, _params.fog.density());
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, _params.fog.distance.lowValue());
  glFogf(GL_FOG_END, _params.fog.distance.highValue());
  glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
  glEnable(GL_FOG);
}

void RenderingController::endFog() {
  glDisable(GL_FOG);
}

#ifdef ENABLE_SYPHON
void RenderingController::pushToSyphon(ofxSyphonServer& syphonServer) {
  _postProc->pushToSyphon(syphonServer);
}
#endif
