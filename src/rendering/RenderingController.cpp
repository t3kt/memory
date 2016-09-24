//
//  RenderingController.cpp
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#include <ofMain.h>
#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../rendering/RenderingController.h"

RenderingController::RenderingController(Params& params,
                                         ofAppGLFWWindow& window,
                                         Context& context)
: _params(params)
, _window(window)
, _colors(AppSystem::get().params()->colors)
, _context(context) {
}

void RenderingController::setup() {
  ofEnableAlphaBlending();
  _camera = std::make_shared<CameraController>(_params.camera,
                                               _context);
  const auto& appParams = *AppSystem::get().params();
  const auto& colors = appParams.colors;
  const auto& observerParams = _params.observers;
  const auto& occurrenceParams = _params.occurrences;
  _observerPreRenderer =
  std::make_shared<ObserverPreRenderer>(observerParams.preRenderer,
                                        colors,
                                        _context);
  _occurrencePreRenderer =
  std::make_shared<OccurrencePreRenderer>(occurrenceParams.preRenderer,
                                          colors,
                                          _context);
  _observerRenderer =
  std::make_shared<ObserverRenderer>(observerParams.renderer,
                                     colors,
                                     _context);
  //  _instancedObserverRenderer =
  //  std::make_shared<InstancedObserverRenderer>(observerParams.instancedRenderer,
  //                                              _context);
  //  _instancedObserverRenderer->setup();
  _observerConnectorRenderer =
  std::make_shared<ObserverObserverConnectorRenderer>(observerParams.connectorRenderer,
                                                      colors.observerConnector.get(),
                                                      _context.observers);
  _occurrenceRenderer =
  std::make_shared<OccurrenceRenderer>(occurrenceParams.renderer,
                                       colors,
                                       _context);
  _observerOccurrenceConnectorRenderer =
  std::make_shared<ObserverOccurrenceConnectorRenderer>(occurrenceParams.connectorRenderer,
                                                        colors.occurrenceObserverConnector.get(),
                                                        _context.occurrences);
  _occurrenceOccurrenceConnectorRenderer =
  std::make_shared<OccurrenceOccurrenceConnectorRenderer>(occurrenceParams.occurrenceConnectorRenderer,
                                                          colors.occurrenceConnector.get(),
                                                          _context.occurrences);
  _postProc = std::make_shared<PostProcController>(_params.postProc);
  _postProc->setup();
  //  _light.setDirectional();
  //  _light.setPosition(ofVec3f(0, 3, 0));
  //  _light.setDiffuseColor(ofFloatColor::red);
  //  _light.setAttenuation(4);
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
  auto& cam = _camera->getCamera();
  cam.begin();
  _context.performActionOnParticleEntityPtrs([&](std::shared_ptr<ParticleObject> entity) {
    entity->setScreenPos(cam.worldToScreen(entity->position()));
  });
  cam.end();
  //  _instancedObserverRenderer->update();
  _postProc->update();
}

void RenderingController::beginDraw() {
  _observerPreRenderer->update();
  _occurrencePreRenderer->update();

  ofBackground(_colors.background.get());
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
  _observerRenderer->draw();
  //  _instancedObserverRenderer->draw();
  _observerConnectorRenderer->draw();
  _occurrenceRenderer->draw();
  _observerOccurrenceConnectorRenderer->draw();
  _occurrenceOccurrenceConnectorRenderer->draw();
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
    const auto& color = _colors.background.get();
    fogCol[0] = color.r;
    fogCol[1] = color.g;
    fogCol[2] = color.b;
  } else {
    const auto& color = _colors.fog.get();
    fogCol[0] = color.r;
    fogCol[1] = color.g;
    fogCol[2] = color.b;
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
