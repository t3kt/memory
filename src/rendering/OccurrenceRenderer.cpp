//
//  OccurrenceRenderer.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofMain.h>
#include "AppAssets.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "OccurrenceRenderer.h"

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

  const auto& mesh = AppAssets::occurrenceMarkerMesh();

  ofPushStyle();
  ofPushMatrix();

  ofSetColor(ofFloatColor(baseColor, baseColor.a * alpha));
  ofTranslate(entity.position());
  ofScale(ofVec3f(size));
  mesh.draw();

  if (_params.wireEnabled()) {
    ofScale(ofVec3f(_params.wireScale()));
    ofFloatColor wireColor(baseColor, baseColor.a * alpha);
    wireColor.setSaturation(_params.wireSaturation());
    wireColor.setBrightness(_params.wireBrightness());
    ofSetColor(wireColor);
    mesh.drawWireframe();
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
