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
  for (auto& entity : _entities) {
    auto alpha = entity->getRemainingLifetimeFraction();
    auto age = entity->getAge(_context.state);
    if (age < fadeIn->getDuration()) {
      alpha *= fadeIn->getValue(age);
    }
    alpha = ofClamp(alpha, 0, 1);
    entity->setAlpha(alpha);
    entity->setSize(size);
  }
}
