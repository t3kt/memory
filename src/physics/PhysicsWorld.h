//
//  PhysicsWorld.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsWorld_h
#define PhysicsWorld_h

#include <functional>
#include "ObjectManager.h"
#include "State.h"

class ObserverEntity;
class OccurrenceEntity;
class ParticleObject;

class PhysicsWorld {
public:
  using EntityAction = std::function<void(ParticleObject*)>;

  PhysicsWorld(const State& state,
               ObjectManager<ObserverEntity>& observers,
               ObjectManager<OccurrenceEntity>& occurrences)
  : _state(state)
  , _observers(observers)
  , _occurrences(occurrences) { }

  const State& state() const { return _state; }

  float time() const { return _state.time; }

  ObjectManager<ObserverEntity>& observers() {
    return _observers;
  }
  const ObjectManager<ObserverEntity>& observers() const {
    return _observers;
  }

  ObjectManager<OccurrenceEntity>& occurrences() {
    return _occurrences;
  }
  const ObjectManager<OccurrenceEntity>& occurrences() const {
    return _occurrences;
  }

  template<typename E>
  ObjectManager<E>& getEntities();

  template<typename E>
  const ObjectManager<E>& getEntities() const;

  void performActionOnAllEntities(EntityAction action);

  template<typename E>
  void performActionEntitiesOfType(EntityAction action) {
    getEntities<E>().performTypedAction(action);
  }
private:
  const State& _state;
  ObjectManager<ObserverEntity>& _observers;
  ObjectManager<OccurrenceEntity>& _occurrences;
};

#endif /* PhysicsWorld_h */
