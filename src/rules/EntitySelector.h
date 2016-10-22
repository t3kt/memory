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
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/WorldObject.h"

template<typename E>
class EntitySelector
: public NonCopyable {
public:
  using ActionT = PtrRefAction<E>;

  EntitySelector(Context& context)
  : _context(context) { }

  virtual void applyAction(ActionT action) {
    _context.performEntityAction<E>(action);
  };
protected:
  Context& _context;
};

template<typename E>
class PredicateEntitySelector
: public EntitySelector<E> {
public:
  using ActionT = typename EntitySelector<E>::ActionT;
  using PredicateT = PtrRefPredicate<E>;

  PredicateEntitySelector(Context& context, PredicateT predicate)
  : EntitySelector<E>(context)
  , _predicate(predicate) { }

  void applyAction(ActionT action) override {
    EntitySelector<E>::_context.performEntityAction([&](std::shared_ptr<E>& entity) {
      if (_predicate(entity)) {
        action(entity);
      }
    });
  }
private:
  PredicateT _predicate;
};

#endif /* EntitySelector_h */
