//
//  NavigatorState.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include "AppParameters.h"
#include "NavigatorState.h"

template<typename E>
std::shared_ptr<E> getRandomEntity(EntityMap<E>& entities) {
  if (entities.empty()) {
    return std::shared_ptr<E>();
  }
  auto index = static_cast<int>(ofRandom(0, entities.size() - 1));
  return entities.getAtIndex(index);
}

template<typename E>
std::shared_ptr<E> getRandomEntity(ObjectManager<E>& entities) {
  if (entities.empty()) {
    return std::shared_ptr<E>();
  }
  auto index = static_cast<int>(ofRandom(0, entities.size() - 1));
  return entities.getAtIndex(index);
}

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
