//
//  PhysicsBehavior.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsBehavior_h
#define PhysicsBehavior_h

#include "../core/Bounds.h"
#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"

class AbstractPhysicsBehavior {
public:
  virtual void applyToWorld(Context& context) = 0;
  virtual void debugDraw(Context& context) {
    beginDebugDraw();
    debugDrawBehavior(context);
    endDebugDraw();
  }
protected:
  virtual void beginDebugDraw() { }
  virtual void debugDrawBehavior(Context& context) {}
  virtual void endDebugDraw() { }

  static void drawForceArrow(ofVec3f position,
                             ofVec3f force);
};

class BoundsBehavior
: public AbstractPhysicsBehavior {
public:
  using Params = ::ParamsWithEnabled;

  BoundsBehavior(const Params& params, const Bounds& bounds)
  : _params(params)
  , _bounds(bounds) { }

  void applyToWorld(Context& context) override;

private:
  void applyToEntity(Context& context, ParticleObject* entity);

  const Params& _params;
  const Bounds& _bounds;
};


#endif /* PhysicsBehavior_h */
