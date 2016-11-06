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
      drawTracer(*connection, renderer);
    }
    for (const auto& connection : entity->getOccurrenceConnections()) {
      drawTracer(*connection, renderer);
    }
  }
  for (const auto& entity : _context.occurrences) {
    for (const auto& connection : entity->getObserverConnections()) {
      drawTracer(*connection, renderer);
    }
    for (const auto& connection : entity->getOccurrenceConnections()) {
      drawTracer(*connection, renderer);
    }
  }
  renderer->popStyle();
}

void ConnectionTracerRenderer
::drawTracer(const AbstractConnection& connection,
             std::shared_ptr<ofBaseRenderer>& renderer) {
  if (!connection.visible()) {
    return;
  }
  auto ratio = ofWrap(_rawRatio
                      + static_cast<float>(connection.entityId() % 12) / 12.0f,
                      0, 1);

  auto tracerPos = connection.evaluatePosition(ratio);
  const auto& entityA = connection.sourceEntity();
  const auto& entityB = connection.targetEntity();
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
