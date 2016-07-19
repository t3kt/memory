//
//  ObserverRenderer.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofMain.h>
#include "AppAssets.h"
#include "ObserverEntity.h"
#include "ObserverRenderer.h"

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
  ofTranslate(entity.position());
  ofScale(ofVec3f(size));
  AppAssets::observerMarkerMesh().draw();

  ofPopMatrix();
  ofPopStyle();
}
