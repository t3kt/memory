//
//  OccurrenceSpawner.cpp
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#include <ofMath.h>
#include "AppSystem.h"
#include "Bounds.h"
#include "Context.h"
#include "Logging.h"
#include "OccurrenceSpawner.h"
#include "OccurrencesController.h"

int OccurrenceSpawnerCore::spawnEntities(Context& context) {
  float startRadius = _params.radius.getValue();
  ofVec3f startPos = _bounds.randomPoint();
  int chainCount = _params.chainCount.getValue();
  if (!_params.chain.getValue() || chainCount <= 1) {
    auto occurrence = spawnEntity(context,
                                  startRadius,
                                  startPos,
                                  nullptr);
    return occurrence ? 1 : 0;
  }
  float endRadius = _params.radius.getValue();
  ofVec3f endPos = startPos + _params.chainDistance.getValue();
  AppSystem::get().log().occurrence().logNotice([&](ofLog& log) {
    log << "Spawning occurrence chain - count: " << chainCount;
    log << ", radius: " << startRadius << "->" << endRadius;
    log << ", pos: " << startPos << "->" << endPos;
  });
  std::shared_ptr<OccurrenceEntity> prev;
  int i;
  for (i = 0; i < chainCount; ++i) {
    float fraction = ofMap(i,
                           0, chainCount - 1,
                           0, 1);
    auto radius = ofLerp(startRadius,
                         endRadius,
                         fraction);
    auto pos = startPos.getInterpolated(endPos, fraction);
    auto occurrence = spawnEntity(context,
                                  radius,
                                  pos,
                                  prev);
    if (!occurrence) {
      break;
    } else {
      prev = occurrence;
    }
  }
  AppSystem::get().log().occurrence().logNotice([&](ofLog& log) {
    log << "Spawned occurrence chain - actual count: " << (i + 1);
  });
  return i + 1;
}

std::shared_ptr<OccurrenceEntity>
OccurrenceSpawnerCore::spawnEntity(Context &context,
                                   float radius,
                                   const ofVec3f &pos,
                                   std::shared_ptr<OccurrenceEntity> prev) {
  float radiusFraction = ofMap(radius,
                               _params.radius.lowValue.get(),
                               _params.radius.highValue.get(),
                               0, 1);
  auto occurrence = std::make_shared<OccurrenceEntity>(pos,
                                                       radius,
                                                       radiusFraction,
                                                       context.state);
  if (_controller.tryAddEntity(occurrence)) {
    if (prev) {
      prev->addOccurrence(occurrence);
    }
    occurrence->setVelocity(_params.initialVelocity.getValue());
    return occurrence;
  }
  return nullptr;
}

void RateOccurrenceSpawner::spawnEntities(Context &context,
                                          int count) {
  for (int i = 0; i < count; ++i) {
    _core.spawnEntities(context);
  }
}

void IntervalOccurrenceSpawner::spawnEntities(Context &context) {
  _core.spawnEntities(context);
}
