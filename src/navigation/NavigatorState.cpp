//
//  NavigatorState.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../navigation/NavigatorState.h"
#include "../physics/BoundsController.h"

void PointNavState::outputFields(std::ostream& os) const {
  os << "point: " << _point;
}

void PointNavState::fillInfo(Info &info,
                             const std::string &prefix) const {
  info.add(prefix + "point:", _point);
}

NavigatorStatePtr PointNavState::nextState(Context &context) {
  const auto& bounds = AppSystem::get().simulation().bounds();
  auto point = bounds.randomPoint();
  return std::make_shared<PointNavState>(point);
}

void ObserverNavState::outputFields(std::ostream& os) const {
  os << "entity: " << *_entity;
}

void ObserverNavState::fillInfo(Info &info,
                                const std::string &prefix) const {
  info.add(prefix + "observer:", _entity->id());
}

void OccurrenceNavState::outputFields(std::ostream &os) const {
  os << "entity: " << *_entity;
}

void OccurrenceNavState::fillInfo(Info &info,
                                  const std::string &prefix) const {
  info.add(prefix + "occurrence:", _entity->id());
}

NavigatorStatePtr ObserverNavState::nextState(Context& context) {
  auto other = _entity->getOccurrenceConnections().getRandomEntity();
  if (!other) {
    return NavigatorStatePtr();
  }
  return std::make_shared<OccurrenceNavState>(other);
}

NavigatorStatePtr OccurrenceNavState::nextState(Context& context) {
  auto other = _entity->getObserverConnections().getRandomEntity();
  if (!other) {
    return NavigatorStatePtr();
  }
  return std::make_shared<ObserverNavState>(other);
}
