//
//  NavigatorState.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include "../app/AppParameters.h"
#include "../navigation/NavigatorState.h"

void PointNavState::outputFields(std::ostream& os) const {
  os << "point: " << _point;
}

NavigatorStatePtr PointNavState::nextState(Context &context) {
  auto point = context.appParams.core.bounds.randomPoint();
  return std::make_shared<PointNavState>(point);
}

void ObserverNavState::outputFields(std::ostream& os) const {
  os << "entity: " << *_entity;
}

NavigatorStatePtr ObserverNavState::nextState(Context& context) {
  auto other = getRandomEntity(_entity->getConnectedObservers());
  if (!other) {
    return NavigatorStatePtr();
  }
  return std::make_shared<ObserverNavState>(other);
}
