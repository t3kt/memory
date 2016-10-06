//
//  OccurrencePreRenderer.cpp
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../rendering/OccurrencePreRenderer.h"

OccurrencePreRenderer::OccurrencePreRenderer(const Params& params,
                                             const ColorTheme& colors,
                                             Context& context)
: EntityPreRenderer(params,
                    colors,
                    context,
                    context.occurrences) { }

void OccurrencePreRenderer::update() {
  _fadeIn.update(_context.state);
  auto fadeIn = _fadeIn.getPhrase();
  auto lowCount = _params.connectionCountRange.lowValue.get();
  auto highCount = _params.connectionCountRange.highValue.get();

  auto darkening = 1.0 - _params.highlightAmount.get();
  auto baseColor = _colors.occurrenceMarker.get();
  auto darkendColor = ofFloatColor(baseColor, baseColor.a * darkening);
  auto hasHighlights = !_context.highlightedEntities.empty();

  for (auto& entity : _entities) {
    if (!entity->alive()) {
      continue;
    }
    ofFloatColor color;
    if (hasHighlights &&
        !_context.highlightedEntities.containsId(entity->id())) {
      color = darkendColor;
    } else {
      color = baseColor;
    }
    color.a *= ofMap(entity->getAmountOfObservation(),
                     lowCount,
                     highCount,
                     0, 1, true);
    auto age = entity->getAge(_context.state);
    if (age < fadeIn->getDuration()) {
      color.a *= fadeIn->getValue(age);
    }
    color.a = ofClamp(color.a, 0, 1);
    entity->setColor(color);
    entity->setSize(ofMap(entity->originalRadiusFraction(),
                          0, 1,
                          _params.sizeRange.lowValue.get(),
                          _params.sizeRange.highValue.get()));
  }
}
