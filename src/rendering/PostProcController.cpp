//
//  PostProcController.cpp
//  memory
//
//  Created by tekt on 7/13/16.
//
//

#include "PostProcController.h"

void PostProcController::setup() {
  _postProc.init();
  _fxaaPass = _postProc.createPass<FxaaPass>();
  _edgePass = _postProc.createPass<EdgePass>();
  _dofPass = _postProc.createPass<DofPass>();
  _rimHighlightPass = _postProc.createPass<RimHighlightingPass>();
  _bloomPass = _postProc.createPass<BloomPass>();
}

void PostProcController::update(const State &state) {
  if (!_params.enabled()) {
    return;
  }
  _fxaaPass->setEnabled(_params.fxaa.enabled());
  if (_params.edge.enabled()) {
    _edgePass->setEnabled(true);
    _edgePass->setHue(_params.edge.hue());
    _edgePass->setSaturation(_params.edge.saturation());
  } else {
    _edgePass->setEnabled(false);
  }
  if (_params.dof.enabled()) {
    _dofPass->setEnabled(true);
    _dofPass->setFocus(_params.dof.focus());
    _dofPass->setAperture(_params.dof.aperture());
    _dofPass->setMaxBlur(_params.dof.maxBlur());
  } else {
    _dofPass->setEnabled(false);
  }
  _rimHighlightPass->setEnabled(_params.rimHighlight.enabled());
  _bloomPass->setEnabled(_params.bloom.enabled());
}

void PostProcController::beginDraw(ofCamera &cam) {
  if (_params.enabled()) {
    _postProc.begin(cam);
  } else {
    cam.begin();
  }
}

void PostProcController::endDraw(ofCamera& cam) {
  if (_params.enabled()) {
    _postProc.end();
  } else {
    cam.end();
  }
}

#ifdef ENABLE_SYPHON
void PostProcController::pushToSyphon(ofxSyphonServer &syphonServer) {
  if (_params.enabled()) {
    syphonServer.publishTexture(&_postProc.getProcessedTextureReference());
  } else {
    syphonServer.publishScreen();
  }
}
#endif
