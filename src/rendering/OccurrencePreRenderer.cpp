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
  for (auto& entity : _entities) {
    auto alpha = ofMap(entity->getAmountOfObservation(),
                       lowCount,
                       highCount,
                       0, 1, true);
    auto age = entity->getAge(_context.state);
    if (age < fadeIn->getDuration()) {
      alpha *= fadeIn->getValue(age);
    }
    alpha = ofClamp(alpha, 0, 1);
    entity->setAlpha(alpha);
  }
}
