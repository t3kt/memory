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
class NavigatorEntity;
class NodeEntity;
class ObserverEntity;
class OccurrenceEntity;
class ParticleObject;
class WorldObject;

class Context
: public NonCopyable
, public JsonWritable
, public JsonReadable {
public:
  float time() const { return state.time; }

  template<typename E>
  ObjectManager<E>& getEntities();

  template<typename E>
  const ObjectManager<E>& getEntities() const;

  template<typename E, typename A>
  void performEntityAction(A action) {
    getEntities<E>().performAction(action);
  }

  void performActionOnParticleEntityPtrs(std::function<void(std::shared_ptr<ParticleObject>)> action);

  Json to_json() const override;
  void read_json(const Json& val) override;

  State state;
  ObjectManager<AnimationObject> animations;
  ObjectManager<NavigatorEntity> navigators;
  ObjectManager<NodeEntity> nodes;
  ObjectManager<ObserverEntity> observers;
  ObjectManager<OccurrenceEntity> occurrences;

  EntityMap<WorldObject> highlightedEntities;

private:
  void clear();
};

#endif /* Context_h */
