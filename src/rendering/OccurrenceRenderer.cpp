//
//  OccurrenceRenderer.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofMain.h>
#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../rendering/OccurrenceRenderer.h"

OccurrenceRenderer::OccurrenceRenderer(const Params& params,
                                       const ColorTheme& colors,
                                       Context& context)
: _context(context)
, _entities(context.occurrences)
, _params(params)
, _colors(colors) { }

void OccurrenceRenderer::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  auto renderer = ofGetCurrentRenderer();

  renderer->pushStyle();

  auto darkening = 1.0 - _params.highlightAmount.get();
  auto baseColor = _colors.occurrenceMarker.get();
  auto darkendColor = ofFloatColor(baseColor, baseColor.a * darkening);
  auto hasHighlights = !_context.highlightedEntities.empty();

  for (const auto& entity : _entities) {
    if (!entity->visible()) {
      continue;
    }

    ofFloatColor color;
    if (hasHighlights &&
        !_context.highlightedEntities.containsId(entity->id())) {
      color = darkendColor;
    } else {
      color = baseColor;
    }
    color.a *= entity->alpha();
    renderer->setColor(color);

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

      ofFloatColor color;
      if (hasHighlights &&
          !_context.highlightedEntities.containsId(entity->id())) {
        color = darkendColor;
      } else {
        color = baseColor;
      }
      color.a *= entity->alpha();

      color.setSaturation(_params.wireSaturation());
      color.setBrightness(_params.wireBrightness());
      renderer->setColor(color);
      renderer->drawBox(entity->position(), size);
    }
  }

  if (_params.showRange.get()) {
    renderer->setBlendMode(OF_BLENDMODE_ALPHA);
    renderer->setFillMode(OF_FILLED);
    auto baseRangeColor = _colors.occurrenceRange.get();
    auto darkenedRangeColor = ofFloatColor(baseRangeColor, baseRangeColor.a * darkening);
    for (const auto& entity : _entities) {
      if (!entity->visible()) {
        continue;
      }

      ofFloatColor color;
      if (hasHighlights &&
          !_context.highlightedEntities.containsId(entity->id())) {
        color = darkenedRangeColor;
      } else {
        color = baseRangeColor;
      }
      color.a *= entity->alpha();
      renderer->setColor(color);
      float radius = entity->actualRadius();
      renderer->drawSphere(entity->position(), radius);
    }
  }

  _context.highlightedEntities.performEntityAction<OccurrenceEntity>([&](std::shared_ptr<OccurrenceEntity>& entity) {
    renderer->setFillMode(OF_OUTLINE);
    renderer->setColor(ofFloatColor(1, 1, 1, 0.5));
    float size = ofMap(entity->originalRadiusFraction(),
                       0, 1,
                       _params.sizeRange.lowValue.get(),
                       _params.sizeRange.highValue.get());
    renderer->drawBox(entity->position(), size * 1.5);
  });

  renderer->popStyle();

}

