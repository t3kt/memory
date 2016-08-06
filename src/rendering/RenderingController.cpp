//
//  RenderingController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include "AppParameters.h"
#include "ConnectorRenderer.h"
#include "ObserverRenderer.h"
#include "OccurrenceRenderer.h"
#include "RenderingController.h"
#include <ofMain.h>

RenderingController::RenderingController(Params& params,
                                         ofAppGLFWWindow& window,
                                         Context& context)
: _params(params)
, _window(window)
, _colors(ColorTheme::get())
, _context(context)
, _backgroundColor(ColorTheme::get().getColor(ColorId::BACKGROUND))
, _fogColor(ColorTheme::get().getColor(ColorId::FOG)) {
}

void RenderingController::setup() {
  ofEnableAlphaBlending();
  _camera = std::make_shared<CameraController>(_params.camera,
                                               _context);
  _camera->setup();
  const auto& appParams = _context.appParams;
  const auto& colors = appParams.colors;
  const auto& observerParams = appParams.observers;
  const auto& occurrenceParams = appParams.occurrences;
  _observerThresholdRenderer =
  std::make_shared<ObserverThresholdRenderer>(_context.observers,
                                              observerParams.threshold,
                                              colors.getColor(ColorId::OBSERVER_THRESHOLD_CONNECTOR));
  _observerRenderer =
  std::make_shared<ObserverRenderer>(observerParams.renderer,
                                     _context.observers);
  //  _instancedObserverRenderer =
  //  std::make_shared<InstancedObserverRenderer>(observerParams.instancedRenderer,
  //                                              _context);
  //  _instancedObserverRenderer->setup();
  _observerConnectorRenderer =
  std::make_shared<ObserverObserverConnectorRenderer>(observerParams.connectorRenderer,
                                                      colors.getColor(ColorId::OBSERVER_CONNECTOR),
                                                      _context.observers);
  _occurrenceRenderer =
  std::make_shared<OccurrenceRenderer>(appParams.occurrences.renderer,
                                       appParams,
                                       _context.occurrences);
  _observerOccurrenceConnectorRenderer =
  std::make_shared<ObserverOccurrenceConnectorRenderer>(appParams.occurrences.connectorRenderer,
                                                        colors.getColor(ColorId::OCCURRENCE_OBSERVER_CONNECTOR),
                                                        _context.occurrences);
  _occurrenceOccurrenceConnectorRenderer =
  std::make_shared<OccurrenceOccurrenceConnectorRenderer>(appParams.occurrences.occurrenceConnectorRenderer,
                                                          colors.getColor(ColorId::OCCURRENCE_CONNECTOR),
                                                          _context.occurrences);
  _postProc = std::make_shared<PostProcController>(_params.postProc);
  _postProc->setup();
  //  _light.setDirectional();
  //  _light.setPosition(ofVec3f(0, 3, 0));
  //  _light.setDiffuseColor(ofFloatColor::red);
  //  _light.setAttenuation(4);
  registerAsActionHandler();
}

void RenderingController::updateResolution() {
  auto size = ofVec2f(_window.getWidth(), _window.getHeight());
  _postProc->updateResolution(size);
}

bool RenderingController::performAction(AppAction action) {
  return false;
}

void RenderingController::update() {
  _camera->update();
  _observerRenderer->update(_context.state);
  //  _instancedObserverRenderer->update();
  _observerThresholdRenderer->update();
  _occurrenceRenderer->update(_context.state);
  _postProc->update();
}

void RenderingController::beginDraw() {
  ofBackground(_backgroundColor);
  glPushAttrib(GL_ENABLE_BIT);
//  ofEnableDepthTest();
  //glEnable(GL_DEPTH_TEST);
//  ofEnableLighting();
  glEnable(GL_CULL_FACE);
//  _light.enable();
  _postProc->beginDraw(_camera->getCamera());
  _camera->applyTransform();
  if (_params.fog.enabled()) {
    beginFog();
  }

  ofPushMatrix();
}

void RenderingController::draw() {
  _observerRenderer->draw(_context.state);
  //  _instancedObserverRenderer->draw();
  _observerConnectorRenderer->draw(_context.state);
  _observerThresholdRenderer->draw();
  _occurrenceRenderer->draw(_context.state);
  _observerOccurrenceConnectorRenderer->draw(_context.state);
  _occurrenceOccurrenceConnectorRenderer->draw(_context.state);
}

void RenderingController::endDraw() {
  ofPopMatrix();
  if (_params.fog.enabled()) {
    endFog();
  }
  _postProc->endDraw(_camera->getCamera());
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
