//
//  ObserverPreRenderer.cpp
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../rendering/ObserverPreRenderer.h"

ObserverPreRenderer::ObserverPreRenderer(const Params& params,
                                         const ColorTheme& colors,
                                         Context& context)
: EntityPreRenderer(params,
                    colors,
                    context,
                    context.observers) { }

void ObserverPreRenderer::update() {
  _fadeIn.update(_context.state);
  auto fadeIn = _fadeIn.getPhrase();
  auto size = _params.size.get();

  auto darkening = 1.0 - _params.highlightAmount.get();
  auto baseColor = _colors.observerMarker.get();
  auto darkenedColor = ofFloatColor(baseColor, baseColor.a * darkening);
  auto hasHighlights = !_context.highlightedEntities.empty();

  for (auto& entity : _entities) {
    if (!entity->alive()) {
      continue;
    }
    ofFloatColor color;
    if (hasHighlights &&
        !_context.highlightedEntities.containsId(entity->id())) {
      color = darkenedColor;
    } else {
      color = baseColor;
    }
    if (entity->sick()) {
      color.setSaturation(0);
    }
    color.a = entity->getRemainingLifetimeFraction();
    auto age = entity->getAge(_context.state);
    if (age < fadeIn->getDuration()) {
      color.a *= fadeIn->getValue(age);
    }
    color.a = ofClamp(color.a, 0, 1);
    entity->setColor(color);
    entity->setSize(size);
  }
}
