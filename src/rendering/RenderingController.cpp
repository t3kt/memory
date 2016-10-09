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
, _colors(AppSystem::get().params().colors)
, _context(context) {
}

void RenderingController::setup() {
  ofEnableAlphaBlending();
  _camera = std::make_shared<CameraController>(_params.camera,
                                               _context);
  const auto& appParams = AppSystem::get().params();
  const auto& colors = appParams.colors;
  const auto& observerParams = _params.observers;
  const auto& occurrenceParams = _params.occurrences;

  _fog = std::make_shared<FogController>(_params.fog);

  _preRenderers.add<ObserverPreRenderer>(observerParams.preRenderer, colors, _context);
  _preRenderers.add<OccurrencePreRenderer>(occurrenceParams.preRenderer, colors, _context);
  _renderers.add<ObserverRenderer>(observerParams.renderer, colors, _context);
//  _renderers.add<InstancedObserverRenderer>(observerParams.instancedRenderer, _context);
  _renderers.add<ObserverObserverConnectorRenderer>(observerParams.connectorRenderer, colors.observerConnector.get(), _context.observers);
  _renderers.add<OccurrenceRenderer>(occurrenceParams.renderer, colors, _context);
  _renderers.add<ObserverOccurrenceConnectorRenderer>(occurrenceParams.connectorRenderer, colors.occurrenceObserverConnector.get(), _context.occurrences);
  _renderers.add<OccurrenceOccurrenceConnectorRenderer>(occurrenceParams.occurrenceConnectorRenderer, colors.occurrenceConnector.get(), _context.occurrences);
  _renderers.add<ConnectionTracerRenderer>(_params.connections,
                                           _context);
  _postProc = std::make_shared<PostProcController>(_params.postProc);
  _postProc->setup();
  //  _light.setDirectional();
  //  _light.setPosition(ofVec3f(0, 3, 0));
  //  _light.setDiffuseColor(ofFloatColor::red);
  //  _light.setAttenuation(4);

  _output =
  std::make_shared<OutputController>(AppSystem::get().params().core.output,
                                     _window);
  _output->setup();
  _output->resolutionChanged += [&](ValueEventArgs<glm::ivec2> e) {
    _postProc->updateResolution(e.value());
  };
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
  _renderers.update();
  _postProc->update();
}

void RenderingController::beginDraw() {
  _preRenderers.update();

  ofBackground(_colors.background.get());
  glPushAttrib(GL_ENABLE_BIT);
//  ofEnableDepthTest();
  //glEnable(GL_DEPTH_TEST);
//  ofEnableLighting();
  glEnable(GL_CULL_FACE);
//  _light.enable();
  _postProc->beginDraw(_camera->getCamera());
  _camera->applyTransform();
  _fog->beginDraw();

  ofPushMatrix();
}

void RenderingController::draw() {
  _renderers.draw();
}

void RenderingController::endDraw() {
  ofPopMatrix();
  _fog->endDraw();
  _postProc->endDraw(_camera->getCamera());
  
//  ofDisableDepthTest();
//  ofDisableLighting();
  glPopAttrib();

  _postProc->pushToOutput(*_output);
}
