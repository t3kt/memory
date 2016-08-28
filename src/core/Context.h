//
//  Context.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef Context_h
#define Context_h

#include <functional>
#include "../core/Common.h"
#include "../core/EntityMap.h"
#include "../core/JsonIO.h"
#include "../core/ObjectManager.h"
#include "../core/Serialization.h"
#include "../core/State.h"

class AnimationObject;
class MemoryAppParameters;
class NavigatorEntity;
class ObserverEntity;
class OccurrenceEntity;
class ParticleObject;
class WorldObject;

class Context
: public NonCopyable
, public JsonWritable
, public JsonReadable {
public:
  Context(MemoryAppParameters& appPars)
  : appParams(appPars) { }

  float time() const { return state.time; }

  template<typename E>
  ObjectManager<E>& getEntities();

  template<typename E>
  const ObjectManager<E>& getEntities() const;

  template<typename E>
  void performActionOnEntitiesOfType(std::function<void(E*)> action) {
    getEntities<E>().performTypedAction(action);
  }

  template<typename E, typename T>
  void performTypedActionOnEntities(std::function<void(T*)> action) {
    getEntities<E>().performTypedAction(action);
  }

  template<typename T>
  void performTypedActionOnAllEntities(std::function<void(T*)> action) {
    performTypedActionOnEntities<AnimationObject>(action);
    performTypedActionOnEntities<NavigatorEntity>(action);
    performTypedActionOnEntities<ObserverEntity>(action);
    performTypedActionOnEntities<OccurrenceEntity>(action);
  }

  void performActionOnParticleEntityPtrs(std::function<void(std::shared_ptr<ParticleObject>)> action);

  Json to_json() const override;
  void read_json(const Json& val) override;

  MemoryAppParameters& appParams;
  State state;
  ObjectManager<AnimationObject> animations;
  ObjectManager<NavigatorEntity> navigators;
  ObjectManager<ObserverEntity> observers;
  ObjectManager<OccurrenceEntity> occurrences;

  EntityMap<WorldObject> highlightedEntities;

private:
  void clear();
};

#endif /* Context_h */
