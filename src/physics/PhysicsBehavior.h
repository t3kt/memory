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
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"

class AbstractPhysicsBehavior
: public NonCopyable {
public:
  AbstractPhysicsBehavior(Context& context)
  : _context(context) { }

  virtual void setup() {}
  virtual void update() = 0;
  virtual void debugDraw() {
    beginDebugDraw();
    debugDrawBehavior();
    endDebugDraw();
  }
protected:
  virtual void beginDebugDraw() { }
  virtual void debugDrawBehavior() {}
  virtual void endDebugDraw() { }

  static void drawForceArrow(ofVec3f position,
                             ofVec3f force);

  Context& _context;
};

class BoundsBehavior
: public AbstractPhysicsBehavior {
public:
  using Params = ::ParamsWithEnabled;

  BoundsBehavior(Context& context,
                 const Params& params,
                 const Bounds& bounds)
  : AbstractPhysicsBehavior(context)
  , _params(params)
  , _bounds(bounds) { }

  void update() override;

private:
  void applyToEntity(ParticleObject* entity);

  const Params& _params;
  const Bounds& _bounds;
};


#endif /* PhysicsBehavior_h */
