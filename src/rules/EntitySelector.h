//
//  EntitySelector.h
//  memory
//
//  Created by tekt on 9/4/16.
//
//

#ifndef EntitySelector_h
#define EntitySelector_h

#include <functional>
#include <memory>
#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/WorldObject.h"

template<typename E>
class EntitySelector {
public:
  using ActionT = PtrAction<E>;

  EntitySelector(Context& context)
  : _context(context) { }

  virtual void applyAction(ActionT action) {
    _context.performEntityAction<E>(action);
  };
protected:
  Context& _context;
};

#endif /* EntitySelector_h */
