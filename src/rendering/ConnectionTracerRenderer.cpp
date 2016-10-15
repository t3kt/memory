//
//  ConnectionTracerRenderer.cpp
//  memory
//
//  Created by tekt on 10/7/16.
//
//

#include <ofAppRunner.h>
#include <ofGraphics.h>
#include <ofMath.h>
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../rendering/ConnectionTracerRenderer.h"

void ConnectionTracerRenderer::update() {
  if (!_params.enabled.get()) {
    return;
  }
  _rawRatio += _context.rootState.timeDelta * _params.rate.get();
}

void ConnectionTracerRenderer::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  auto renderer = ofGetCurrentRenderer();
  renderer->pushStyle();
  renderer->setFillMode(OF_OUTLINE);
  for (const auto& entity : _context.observers) {
    for (const auto& connection : entity->getObserverConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
    for (const auto& connection : entity->getOccurrenceConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
  }
  for (const auto& entity : _context.occurrences) {
    for (const auto& connection : entity->getObserverConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
    for (const auto& connection : entity->getOccurrenceConnections()) {
      const auto& other = connection->entity();
      drawTracer(*entity, *other, renderer);
    }
  }
  renderer->popStyle();
}

void ConnectionTracerRenderer::drawTracer(ParticleObject& entityA,
                                          ParticleObject& entityB,
                                          std::shared_ptr<ofBaseRenderer>& renderer) {
  if (!entityA.visible() || !entityB.visible()) {
    return;
  }
  auto ratio = ofWrap(_rawRatio
                      + static_cast<float>(entityA.id() % 12) / 12.0f,
                      0, 1);

  const auto& posA = entityA.position();
  const auto& posB = entityB.position();
  auto tracerPos = getInterpolated(posA, posB, ratio);
  auto color = getInterpolated(entityA.color(), entityB.color(), ratio);
  color.setBrightness(color.getBrightness() * 1.2);
  color.a *= getInterpolated(entityA.alpha(),
                             entityB.alpha(),
                             ratio);
  color.a *= _params.alphaFade.evaluate(ratio);
  renderer->setColor(color);
  renderer->drawBox(tracerPos,
                    _params.drawRadius.get());
}
