//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "EntityRenderer.h"
#include <ofMain.h>

EnumTypeInfo<EntityShape> EntityShapeType({
  {"sphere", EntityShape::SPHERE},
  {"box", EntityShape::BOX},
});

AbstractEntityRenderer::Params::Params()
: ::Params() {
  add(size
      .setKey("size")
      .setName("Draw Size")
      .setValueAndDefault(0.03)
      .setRange(0, 0.1));
}

template<typename T>
void EntityRenderer<T>::draw(const State& state) {
  ofFloatColor baseColor(_color);
  float size = _baseParams.size.get();
  ofPushStyle();
  ofFill();
  for (shared_ptr<T> entity : *this) {
    if (!entity->visible()) {
      continue;
    }
    drawEntity(*entity, baseColor, size, state);
  }
  ofPopStyle();
}

void ObserverRenderer::drawEntity(const ObserverEntity &entity, const ofFloatColor &baseColor, float size, const State& state) const {
  float alpha = entity.getRemainingLifetimeFraction();
  if (alpha <= 0) {
    return;
  }
  ofSetColor(ofFloatColor(baseColor, baseColor.a * alpha));
  ofDrawSphere(entity.position(), size);
}

OccurrenceRenderer::Params::Params()
: AbstractEntityRenderer::Params() {
  add(showRange
      .setKey("showRange")
      .setName("Show Range")
      .setValueAndDefault(true));
  add(connectionCountRange
      .setKey("connectionCountRange")
      .setName("Connection Amount Scale")
      .setParamValuesAndDefaults(0, 4)
      .setParamRanges(0, 20));
  size.setValueAndDefault(0.1);
  size.setRange(0, 0.5);
}

void OccurrenceRenderer::drawEntity(const OccurrenceEntity &entity, const ofFloatColor &baseColor, float size, const State& state) const {
  auto count = entity.getAmountOfObservation(state);
  float alpha = ofMap(count,
                      _params.connectionCountRange.lowValue.get(),
                      _params.connectionCountRange.highValue.get(),
                      0, 1, true);
  float fadeIn = entity.getFadeInPercent(state);
  alpha *= fadeIn;
  if (alpha <= 0.0) {
    return;
  }

  ofSetColor(ofFloatColor(baseColor, baseColor.a * alpha));
  ofDrawBox(entity.position(), size);

  if (_params.showRange.get()) {
    ofFloatColor rangeColor(_rangeColor);
    rangeColor.a *= alpha;
    if (rangeColor.a > 0) {
      ofPushStyle();
      ofEnableAlphaBlending();
      ofSetColor(rangeColor);
      float radius = entity.actualRadius();
      ofDrawSphere(entity.position(), radius);
      ofPopStyle();
    }
  }
}

ObserverOccurrenceConnectorRenderer::Params::Params()
: ::Params() {
  add(enabled
      .setKey("enabled")
      .setName("Enabled")
      .setValueAndDefault(true));
  add(connectionCountRange
      .setKey("connectionCountRange")
      .setName("Connection Count Range")
      .setParamValuesAndDefaults(0, 4)
      .setParamRanges(0, 20));
}

void ObserverOccurrenceConnectorRenderer::draw(const State& state) {
  if (!_params.enabled.get()) {
    return;
  }
  ofPushStyle();
  ofMesh connectorMesh;
  float lowCount = _params.connectionCountRange.lowValue.get();
  float highCount = _params.connectionCountRange.highValue.get();
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (auto occurrence : _occurrences) {
    float occurrenceLife = occurrence->getAmountOfObservation(state);
    float occurrenceAlpha = ofMap(occurrenceLife,
                                  lowCount,
                                  highCount,
                                  0, 1, true);
    if (occurrenceAlpha <= 0) {
      continue;
    }
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceAlpha);
    for (auto entry : occurrence->connectedObservers()) {
      auto observer = entry.second;
      float observerLife = observer->getRemainingLifetimeFraction();
      if (observerLife <= 0) {
        continue;
      }
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(ofFloatColor(_color, _color.a * observerLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}