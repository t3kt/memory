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
  const Params& _params;
  const ColorTheme& _colors;
  ofEasyCam _cam;
  ofxPostProcessing _postProc;
//  ofLight _light;
  ofVec3f _rotation;
};

RenderingControllerImpl::RenderingControllerImpl(const Params& params, const ColorTheme& colors)
: _params(params)
, _colors(colors) {

  ofEnableAlphaBlending();
  ofDisableDepthTest();
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
  ofBackground(_colors.getColor(ColorId::BACKGROUND));
  glPushAttrib(GL_ENABLE_BIT);
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
//  _light.enable();
  _postProc.begin(_cam);

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
  _postProc.end();
  glPopAttrib();
}

shared_ptr<RenderingController> RenderingController::create(const Params& params, const ColorTheme& colors) {
  return std::shared_ptr<RenderingController>(new RenderingControllerImpl(params, colors));
}
