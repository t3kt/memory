//
//  OccurrenceSpawner.cpp
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#include <ofMath.h>
#include "Bounds.h"
#include "Context.h"
#include "OccurrenceSpawner.h"
#include "OccurrencesController.h"

void RateOccurrenceSpawner::spawnEntities(Context &context,
                                          int count) {
  for (int i = 0; i < count; ++i) {
    auto pos = _bounds.randomPoint();
    auto radius = _params.radius.getValue();
    auto radiusFraction = ofMap(radius,
                                _params.radius.lowValue.get(),
                                _params.radius.highValue.get(),
                                0, 1);
    auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                         radius,
                                                         radiusFraction,
                                                         context.state);
    occurrence->setVelocity(_params.initialVelocity.getValue());
    _controller.tryAddEntity(occurrence);
  }
}

void IntervalOccurrenceSpawner::spawnEntities(Context &context) {
  auto pos = _bounds.randomPoint();
  auto radius = _params.radius.getValue();
  auto radiusFraction = ofMap(radius,
                              _params.radius.lowValue.get(),
                              _params.radius.highValue.get(),
                              0, 1);
  auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                       radius,
                                                       radiusFraction,
                                                       context.state);
  occurrence->setVelocity(_params.initialVelocity.getValue());
  _controller.tryAddEntity(occurrence);
}
