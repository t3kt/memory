//
//  OccurrencePreRenderer.cpp
//  memory
//
//  Created by tekt on 8/7/16.
//
//

#include "Context.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "OccurrencePreRenderer.h"

OccurrencePreRenderer::OccurrencePreRenderer(const Params& params,
                                             Context& context)
: EntityPreRenderer(params,
                    context,
                    context.occurrences) { }

void OccurrencePreRenderer::update() {
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
