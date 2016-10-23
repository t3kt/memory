//
//  Context.h
//

#pragma once

#include <functional>
#include "../core/Common.h"
#include "../core/JsonIO.h"
#include "../core/ObjectManager.h"
#include "../core/Serialization.h"
#include "../core/State.h"
#include "../graph/EntityGroup.h"

class AbstractParamTransition;
class AnimationObject;
class NavigatorEntity;
class NodeEntity;
class ObserverEntity;
class OccurrenceEntity;
class ParticleObject;
class WorldObject;

// State of the simulation and the app in general.
// Owns the ObjectManager instances which own all entities.
class Context
: public NonCopyable
, public JsonWritable
, public JsonReadable {
public:
  float time() const { return rootState.localTime; }

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

  ClockState rootState;
  ClockState entityState;
  ClockState physicsState;

  ObjectManager<AnimationObject> animations;
  ObjectManager<NavigatorEntity> navigators;
  ObjectManager<NodeEntity> nodes;
  ObjectManager<ObserverEntity> observers;
  ObjectManager<OccurrenceEntity> occurrences;

  EntityGroup highlightedEntities;

  std::shared_ptr<AbstractParamTransition> activeTransition;

private:
  void clear();
};

