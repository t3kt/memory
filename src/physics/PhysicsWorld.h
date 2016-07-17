//
//  PhysicsWorld.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsWorld_h
#define PhysicsWorld_h

#include "Context.h"
#include "State.h"

class PhysicsWorld {
public:
  PhysicsWorld(Context& context)
  : _context(context) { }

  Context& context() { return _context; }

private:
  Context& _context;
};

#endif /* PhysicsWorld_h */
