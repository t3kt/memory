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
  ofVec3f calcAttractionForce(ParticleObject* entity,
                              const ofVec3f& otherPosition) override;
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
    for (auto entity : world->getEntities<E>()) {
      if (!entity->alive()) {
        continue;
      }
      ofVec3f force = calcAttractionForce(entity.get(),
                                          entity->startPosition());
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
