//
//  Context.h
//

#pragma once

#include <functional>
#include <ofxTJsonIO.h>
#include "../core/Common.h"
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
class Scene;
class WorldObject;

// State of the simulation and the app in general.
// Owns the ObjectManager instances which own all entities.
class Context
: public NonCopyable
, public ofxTCommon::JsonWritable
, public ofxTCommon::JsonReadable {
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

  ofJson toJson() const override;
  void readJson(const ofJson& val) override;

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

  std::shared_ptr<Scene> activeScene;

  bool spawningSuspended;

private:
  void clear();
};

