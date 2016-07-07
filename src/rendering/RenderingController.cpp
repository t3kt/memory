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
  shared_ptr<FxaaPass> _fxaaPass;
  shared_ptr<EdgePass> _edgePass;
  shared_ptr<DofPass> _dofPass;
  shared_ptr<RimHighlightingPass> _rimHighlightPass;
  shared_ptr<BloomPass> _bloomPass;
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
  _fxaaPass = _postProc.createPass<FxaaPass>();
  _edgePass = _postProc.createPass<EdgePass>();
  _dofPass = _postProc.createPass<DofPass>();
  _rimHighlightPass = _postProc.createPass<RimHighlightingPass>();
  _bloomPass = _postProc.createPass<BloomPass>();
//  _light.setDirectional();
//  _light.setPosition(ofVec3f(0, 3, 0));
//  _light.setDiffuseColor(ofFloatColor::red);
//  _light.setAttenuation(4);
}

void RenderingControllerImpl::update(const State &state) {
  if (_params.camera.spinEnabled()) {
    _rotation += _params.camera.spinRate() * state.timeDelta;
  }
  if (_params.postProc.enabled()) {
    _fxaaPass->setEnabled(_params.postProc.fxaa.enabled());
    if (_params.postProc.edge.enabled()) {
      _edgePass->setEnabled(true);
      _edgePass->setHue(_params.postProc.edge.hue());
      _edgePass->setSaturation(_params.postProc.edge.saturation());
    } else {
      _edgePass->setEnabled(false);
    }
    if (_params.postProc.dof.enabled()) {
      _dofPass->setEnabled(true);
      _dofPass->setFocus(_params.postProc.dof.focus());
      _dofPass->setAperture(_params.postProc.dof.aperture());
      _dofPass->setMaxBlur(_params.postProc.dof.maxBlur());
    } else {
      _dofPass->setEnabled(false);
    }
    _rimHighlightPass->setEnabled(_params.postProc.rimHighlight.enabled());
    _bloomPass->setEnabled(_params.postProc.bloom.enabled());
  }
}

void RenderingControllerImpl::beginDraw(const State &state) {
  ofBackground(_backgroundColor);
  glPushAttrib(GL_ENABLE_BIT);
//  ofEnableDepthTest();
  //glEnable(GL_DEPTH_TEST);
//  ofEnableLighting();
  glEnable(GL_CULL_FACE);
//  _light.enable();
  if (_params.postProc.enabled()) {
    _postProc.begin(_cam);
  } else {
    _cam.begin();
  }
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
  if (_params.postProc.enabled()) {
    _postProc.end();
  } else {
    _cam.end();
  }
//  ofDisableDepthTest();
//  ofDisableLighting();
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
