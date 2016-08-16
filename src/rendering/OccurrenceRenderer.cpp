//
//  OccurrenceRenderer.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofMain.h>
#include "../app/AppAssets.h"
#include "../app/AppParameters.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../rendering/OccurrenceRenderer.h"

OccurrenceRenderer::OccurrenceRenderer(const Params& params,
                                       const MemoryAppParameters& appParams,
                                       Context& context)
: AbstractEntityRenderer(params,
                         ColorTheme::get().getColor(ColorId::OCCURRENCE_MARKER),
                         context)
, _entities(context.occurrences)
, _params(params)
, _rangeColor(ColorTheme::get().getColor(ColorId::OCCURRENCE_RANGE))
, _appParams(appParams) { }

void OccurrenceRenderer::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  const auto& mesh = AppAssets::occurrenceMarkerMesh();
  auto renderer = ofGetCurrentRenderer();

  renderer->pushStyle();

  for (const auto& entity : _entities) {
    if (!entity->visible()) {
      continue;
    }

    float alpha = entity->alpha();
    renderer->setColor(ofFloatColor(_color, _color.a * alpha));

    float size = ofMap(entity->originalRadiusFraction(),
                       0, 1,
                       _params.sizeRange.lowValue.get(),
                       _params.sizeRange.highValue.get());

    renderer->drawBox(entity->position(), size);
  }

  if (_params.wireEnabled.get()) {
    renderer->setFillMode(OF_OUTLINE);
    for (const auto& entity : _entities) {
      if (!entity->visible()) {
        continue;
      }

      float size = ofMap(entity->originalRadiusFraction(),
                         0, 1,
                         _params.sizeRange.lowValue.get(),
                         _params.sizeRange.highValue.get());
      size *= _params.wireScale.get();

      ofFloatColor wireColor(_color, _color.a * entity->alpha());
      wireColor.setSaturation(_params.wireSaturation());
      wireColor.setBrightness(_params.wireBrightness());
      renderer->setColor(wireColor);
      renderer->drawBox(entity->position(), size);
    }
  }

  if (_params.showRange.get()) {
    renderer->setBlendMode(OF_BLENDMODE_ALPHA);
    renderer->setFillMode(OF_FILLED);
    for (const auto& entity : _entities) {
      if (!entity->visible()) {
        continue;
      }

      ofFloatColor color(_rangeColor, _rangeColor.a * entity->alpha());
      if (color.a <= 0) {
        continue;
      }
      renderer->setColor(color);
      float radius = entity->actualRadius();
      renderer->drawSphere(entity->position(), radius);
    }
  }

  renderer->popStyle();

}

