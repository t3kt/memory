//
//  RenderingController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "RenderingController.h"
#include <ofEasyCam.h>
#include <ofxPostProcessing.h>
#include <ofMain.h>

RenderingController::Params::Params()
: ::Params() {
  add(camera
      .setKey("camera")
      .setName("Camera"));
  add(fog
      .setKey("fog")
      .setName("Fog"));
}

CameraParams::CameraParams()
: ::Params() {
  add(_spinEnabled
      .setKey("spinEnabled")
      .setName("Spin Enabled")
      .setValueAndDefault(false));
  add(_spinRate
      .setKey("spinRate")
      .setName("Spin Rate")
      .setValueAndDefault(ofVec3f(2, 4, 5))
      .setRange(ofVec3f(-10), ofVec3f(10)));
}

FogParams::FogParams()
: ::Params() {
  add(_enabled
      .setKey("enabled")
      .setName("Enabled")
      .setValueAndDefault(true));
  add(_density
      .setKey("density")
      .setName("Density")
      .setValueAndDefault(0.001f)
      .setRange(0, 0.004f));
  add(_useBackgroundColor
      .setKey("useBackgroundColor")
      .setName("Use Background Color")
      .setValueAndDefault(true));
  add(distance
      .setKey("distance")
      .setName("Distance")
      .setParamValuesAndDefaults(0.3, 0.9)
      .setParamRanges(0, 8));
}

class RenderingControllerImpl
: public RenderingController {
public:
  RenderingControllerImpl(const Params& params,
                          const ColorTheme& colors);

  void update(const State& state) override;

  void beginDraw(const State& state) override;

  void endDraw(const State& state) override;

  void resetCamera() override {
    _cam.reset();
  }

private:
  void beginFog();
  void endFog();

  const Params& _params;
  const ColorTheme& _colors;
  const ofFloatColor& _backgroundColor;
  const ofFloatColor& _fogColor;
  ofEasyCam _cam;
  ofxPostProcessing _postProc;
//  ofLight _light;
  ofVec3f _rotation;
};

RenderingControllerImpl::RenderingControllerImpl(const Params& params, const ColorTheme& colors)
: _params(params)
, _colors(colors)
, _backgroundColor(colors.getColor(ColorId::BACKGROUND))
, _fogColor(colors.getColor(ColorId::FOG)) {

  ofEnableAlphaBlending();
  _postProc.init();
  _postProc.createPass<BloomPass>()->setEnabled(true);
//  _light.setPointLight();
//  _light.setDiffuseColor(ofFloatColor::red);
//  _light.setAttenuation(4);
}

void RenderingControllerImpl::update(const State &state) {
  if (_params.camera.spinEnabled()) {
    _rotation += _params.camera.spinRate() * state.timeDelta;
  }
}

void RenderingControllerImpl::beginDraw(const State &state) {
  ofBackground(_backgroundColor);
  glPushAttrib(GL_ENABLE_BIT);
//  ofEnableDepthTest();
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
//  _light.enable();
  _postProc.begin(_cam);
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

void RenderingControllerImpl::endDraw(const State &state) {
  ofPopMatrix();
  if (_params.fog.enabled()) {
    endFog();
  }
  _postProc.end();
//  ofDisableDepthTest();
  glPopAttrib();
}

void RenderingControllerImpl::beginFog() {
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

void RenderingControllerImpl::endFog() {
  glDisable(GL_FOG);
}

shared_ptr<RenderingController> RenderingController::create(const Params& params, const ColorTheme& colors) {
  return std::shared_ptr<RenderingController>(new RenderingControllerImpl(params, colors));
}
