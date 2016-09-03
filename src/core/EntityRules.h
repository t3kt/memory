//
//  EntityRules.h
//  memory
//
//  Created by tekt on 9/2/16.
//
//

#ifndef EntityRules_h
#define EntityRules_h

#include <memory>
#include <vector>
#include "../core/EntityMap.h"

class AbstractEntityRule {
public:
};

template<typename E>
class EntityRule
: public AbstractEntityRule {
public:
  virtual void applyToEntity(E& entity) = 0;
};

template<typename E>
using EntityRulePtr = std::shared_ptr<EntityRule<E>>;

template<typename E>
using EntityRuleList = std::vector<EntityRulePtr<E>>;

#endif /* EntityRules_h */
