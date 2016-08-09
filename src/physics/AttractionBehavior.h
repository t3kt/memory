//
//  AttractionBehavior.h
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#ifndef AttractionBehavior_h
#define AttractionBehavior_h

#include "Context.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"

class RangedForceParams
: public ParamsWithEnabled {
public:
  RangedForceParams() {
    add(distanceBounds
        .setKey("distanceBounds")
        .setName("Distance Bounds")
        .setParamValuesAndDefaults(5, 100)
        .setParamRanges(0, 500)
        .setParamNames("Near", "Far"));
    add(magnitude
        .setKey("magnitude")
        .setName("Magnitude")
        .setValueAndDefault(4)
        .setRange(0, 20));
    add(reverse
        .setKey("reverse")
        .setName("Reverse")
        .setValueAndDefault(false));
    setEnabledValueAndDefault(true);
  }

  float signedMagnitude() const {
    return magnitude() * (reverse() ? -1 : 1);
  }

  FloatValueRange distanceBounds;
  TParam<float> magnitude;
  TParam<bool> reverse;
};

class AbstractAttractionBehavior
: public AbstractPhysicsBehavior {
public:
  using Params = RangedForceParams;

  AbstractAttractionBehavior(const Params& params)
  : _params(params) { }

  void applyToWorld(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    processWorld(context, ApplyMode::ADD_FORCE);
  }

protected:
  enum class ApplyMode {
    ADD_FORCE,
    DEBUG_DRAW,
  };

  void debugDrawBehavior(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    processWorld(context, ApplyMode::DEBUG_DRAW);
  }

  virtual void processWorld(Context& context,
                            ApplyMode mode) = 0;

  ofVec3f calcAttractionForce(const ofVec3f& entityPosition,
                              const ofVec3f& otherPosition,
                              float lowBound,
                              float highBound,
                              float magnitude);

  void beginDebugDraw() override;
  void endDebugDraw() override;

  virtual void debugDrawEntity(ParticleObject* entity,
                               const ofVec3f& force);
  void debugDrawEntityRange(ParticleObject* entity);

  const Params& _params;

};

template<typename E, typename O>
class AttractionBehavior
: public AbstractAttractionBehavior {
public:
  AttractionBehavior(const Params& params)
  : AbstractAttractionBehavior(params) { }

protected:
  void processWorld(Context& context, ApplyMode mode) override {
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float magnitude = _params.signedMagnitude();
    for (auto& entity : context.getEntities<E>()) {
      if (!entity->alive()) {
        continue;
      }
      for (const auto& other : getEntityOthers(entity.get())) {
        if (!other.second->alive()) {
          continue;
        }
        ofVec3f force = calcAttractionForce(entity->position(),
                                            other.second->position(),
                                            lowBound,
                                            highBound,
                                            magnitude);
        if (force.length() == 0) {
          continue;
        }
        switch (mode) {
          case ApplyMode::ADD_FORCE:
            entity->addForce(force);
            break;
          case ApplyMode::DEBUG_DRAW:
            debugDrawEntity(entity.get(), force);
            break;
        }
      }
    }
  }

  EntityMap<O>& getEntityOthers(E *entity);
};

template<>
class AttractionBehavior<ObserverEntity, ObserverEntity>
: public AbstractAttractionBehavior {
public:
  AttractionBehavior(const Params& params)
  : AbstractAttractionBehavior(params) { }

protected:
  void processWorld(Context& context, ApplyMode mode) override {
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float magnitude = _params.signedMagnitude();
    for (auto& entity : context.getEntities<ObserverEntity>()) {
      if (!entity->alive()) {
        continue;
      }
      for (const auto& other : entity->getConnectedObservers()) {
        if (!other.second->alive()) {
          continue;
        }
        ofVec3f force = calcAttractionForce(entity->position(),
                                            other.second->position(),
                                            lowBound,
                                            highBound,
                                            magnitude);
        if (force.length() == 0) {
          continue;
        }
        switch (mode) {
          case ApplyMode::ADD_FORCE:
            entity->addForce(force);
            break;
          case ApplyMode::DEBUG_DRAW:
            if (!context.highlightedEntities.empty() &&
                !context.highlightedEntities.containsId(entity->id())) {
              break;
            }
            debugDrawEntity(entity.get(), force);
            break;
        }
      }
    }
  }
};


#endif /* AttractionBehavior_h */
