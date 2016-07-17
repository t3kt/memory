//
//  AnchorPointBehavior.h
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#ifndef AnchorPointBehavior_h
#define AnchorPointBehavior_h

#include "AttractionBehavior.h"
#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"
#include "PhysicsWorld.h"

class AbstractAnchorPointBehavior
: public AbstractAttractionBehavior {
public:
  AbstractAnchorPointBehavior(const Params& params)
  : AbstractAttractionBehavior(params) { }
protected:
  ofVec3f calcAttractionForce(const ofVec3f& entityPosition,
                              const ofVec3f& anchorPosition,
                              float lowBound,
                              float highBound,
                              float magnitude,
                              float midDist);
  void debugDrawEntity(ParticleObject* entity,
                       const ofVec3f& force) override;
  virtual void drawOriginShape(const ofVec3f& position) = 0;
};

template<typename E>
class AnchorPointBehavior
: public AbstractAnchorPointBehavior {
public:
  AnchorPointBehavior(const Params& params)
  : AbstractAnchorPointBehavior(params) { }

protected:
  void processWorld(PhysicsWorld* world, ApplyMode mode) override {
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float magnitude = _params.signedMagnitude();
    float midDist = getInterpolated(lowBound, highBound, 0.5);
    for (auto& entity : world->context().getEntities<E>()) {
      if (!entity->alive()) {
        continue;
      }
      ofVec3f force = calcAttractionForce(entity->position(),
                                          entity->startPosition(),
                                          lowBound,
                                          highBound,
                                          magnitude,
                                          midDist);
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
  void drawOriginShape(const ofVec3f& position) override;
};


#endif /* AnchorPointBehavior_h */
