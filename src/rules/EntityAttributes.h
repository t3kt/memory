//
//  EntityAttributes.h
//  memory
//
//  Created by tekt on 8/23/16.
//
//

#ifndef EntityAttributes_h
#define EntityAttributes_h

#include <ofTypes.h>
#include "../rules/EntityRule.h"

template<typename E, typename T>
class EntityAttribute {
public:
  using AttrT = EntityAttribute<E, T>;
  using RuleSeqT = EntityRuleSequence<E, T>;
  using RulePtrT = typename RuleSeqT::RulePtr;

  EntityAttribute(const T& baseValue)
  : _baseValue(baseValue) { }

  AttrT& addRule(RulePtrT rule) {
    _rules.add(rule);
    return *this;
  }

  T calculateValue(std::shared_ptr<E> entity) {
    return _rules.calculateValue(entity, _baseValue);
  }

private:
  const T& _baseValue;
  RuleSeqT _rules;
};

template<typename E>
class EntityAttributes {
public:
  EntityAttribute<E, ofFloatColor> color;
};

/*
 rules:
 - start with color based on param
 - apply alpha multiplier based on fade in for entities within fade in duration
 - apply alpha multiplier based on observer life
 - apply alpha addition to highlighted entities
 - apply alpha limit to non-highlighted entities
 
 rules:
 - start with size based on param
 - apply size multiplier based on occurrence range
*/

#endif /* EntityAttributes_h */
