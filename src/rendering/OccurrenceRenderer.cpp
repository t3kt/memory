//
//  OccurrenceRenderer.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofMain.h>
#include "AppAssets.h"
#include "AppParameters.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "OccurrenceRenderer.h"

OccurrenceRenderer::OccurrenceRenderer(const Params& params,
                                       const MemoryAppParameters& appParams,
                                       ObjectManager<OccurrenceEntity>& entities)
: EntityRenderer<OccurrenceEntity>(params, ColorTheme::get().getColor(ColorId::OCCURRENCE_MARKER))
, _params(params)
, _rangeColor(ColorTheme::get().getColor(ColorId::OCCURRENCE_RANGE))
, _appParams(appParams)
, _entities(entities) { }

void OccurrenceRenderer::drawEntity(const OccurrenceEntity &entity,
                                    const State& state) {
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

  ofSetColor(ofFloatColor(_color, _color.a * alpha));
  ofTranslate(entity.position());

  float size = ofMap(entity.originalRadius(),
                     _appParams.occurrences.radius.lowValue.get(),
                     _appParams.occurrences.radius.highValue.get(),
                     _params.sizeRange.lowValue.get(),
                     _params.sizeRange.highValue.get());

  ofScale(ofVec3f(size));
  mesh.draw();

  if (_params.wireEnabled()) {
    ofScale(ofVec3f(_params.wireScale()));
    ofFloatColor wireColor(_color, _color.a * alpha);
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
