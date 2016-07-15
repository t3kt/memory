//
//  EntityRenderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "EntityRenderer.h"
#include <ofMain.h>

using namespace ofxChoreograph;

EnumTypeInfo<EntityShape> EntityShapeType({
  {"sphere", EntityShape::SPHERE},
  {"box", EntityShape::BOX},
});

void AbstractEntityRenderer::update(const State &state) {
  _fadeIn.update(state);
}

ObserverRenderer::ObserverRenderer(const ObserverRenderer::Params& params, const ColorTheme& colors, ObjectManager<ObserverEntity>& entities)
: EntityRenderer<ObserverEntity>(params,
                                 colors.getColor(ColorId::OBSERVER_MARKER))
, _entities(entities) {
}

void ObserverRenderer::drawEntity(const ObserverEntity &entity, const ofFloatColor &baseColor, float size, const State& state) {
  float alpha = entity.getRemainingLifetimeFraction();
  float age = entity.getAge(state);
  auto fadeIn = _fadeIn.getPhrase();
  if (age < fadeIn->getDuration()) {
    alpha *= fadeIn->getValue(age);
  }
  if (alpha <= 0) {
    return;
  }

  ofPushStyle();
  ofPushMatrix();

  ofSetColor(ofFloatColor(baseColor, baseColor.a * alpha));
  ofDrawSphere(entity.position(), size);

  ofPopMatrix();
  ofPopStyle();
}

void OccurrenceRenderer::drawEntity(const OccurrenceEntity &entity, const ofFloatColor &baseColor, float size, const State& state) {
  auto count = entity.getAmountOfObservation();
  float alpha = ofMap(count,
                      _params.connectionCountRange.lowValue(),
                      _params.connectionCountRange.highValue(),
                      0, 1, true);
  float age = entity.getAge(state);
  auto fadeIn = _fadeIn.getPhrase();
  if (age < fadeIn->getDuration()) {
    alpha *= fadeIn->getValue(age);
  }
  if (alpha <= 0.0) {
    return;
  }

  ofPushStyle();
  ofPushMatrix();

  ofSetColor(ofFloatColor(baseColor, baseColor.a * alpha));
  ofTranslate(entity.position());
  ofScale(ofVec3f(size));
  _markerMesh.draw();

  if (_params.wireEnabled()) {
    ofScale(ofVec3f(_params.wireScale()));
    ofFloatColor wireColor(baseColor, baseColor.a * alpha);
    wireColor.setSaturation(_params.wireSaturation());
    wireColor.setBrightness(_params.wireBrightness());
    ofSetColor(wireColor);
    _markerMesh.drawWireframe();
  }

  ofPopMatrix();
  ofPopStyle();


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

void ObserverOccurrenceConnectorRenderer::draw(const State& state) {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofMesh connectorMesh;
  float lowCount = _params.connectionCountRange.lowValue();
  float highCount = _params.connectionCountRange.highValue();
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _occurrences) {
    float occurrenceLife = occurrence->getAmountOfObservation();
    float occurrenceAlpha = ofMap(occurrenceLife,
                                  lowCount,
                                  highCount,
                                  0, 1, true);
    if (occurrenceAlpha <= 0) {
      continue;
    }
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceAlpha);
    for (const auto& entry : occurrence->connectedObservers()) {
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

void ObserverObserverConnectorRenderer::draw(const State& state) {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& observer : _entities) {
    if (!observer->alive()) {
      continue;
    }
    float observerLife = observer->getRemainingLifetimeFraction();
    ofFloatColor connectorStartColor(_color, _color.a * observerLife);
    for (const auto& other : observer->getConnectedObservers()) {
      if (!other.second->alive()) {
        continue;
      }
      float otherLife = other.second->getRemainingLifetimeFraction();
      connectorMesh.addVertex(observer->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * otherLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}

void OccurrenceOccurrenceConnectorRenderer::draw(const State& state) {
  if (!_params.enabled()) {
    return;
  }
  ofPushStyle();
  ofEnableAlphaBlending();
  ofMesh connectorMesh;
  connectorMesh.setMode(OF_PRIMITIVE_LINES);
  for (const auto& occurrence : _entities) {
    if (!occurrence->alive()) {
      continue;
    }
    float occurrenceLife = occurrence->getAmountOfObservation();
    ofFloatColor connectorStartColor(_color, _color.a * occurrenceLife);
    for (const auto& other : occurrence->getConnectedOccurrences()) {
      if (!other.second->alive()) {
        continue;
      }
      float otherLife = other.second->getAmountOfObservation();
      connectorMesh.addVertex(occurrence->position());
      connectorMesh.addColor(connectorStartColor);
      connectorMesh.addVertex(other.second->position());
      connectorMesh.addColor(ofFloatColor(_color,
                                          _color.a * otherLife));
    }
  }
  connectorMesh.draw();
  ofPopStyle();
}
