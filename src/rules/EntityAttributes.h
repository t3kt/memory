//
//  EntityAttributes.h
//  memory
//
//  Created by tekt on 8/23/16.
//
//

#ifndef EntityAttributes_h
#define EntityAttributes_h

#include "../rules/EntityRule.h"

template<typename E, typename T>
class EntityAttribute {
public:
  using AttrT = EntityAttribute<E, T>;
  using RuleSeqT = EntityRuleSequence<E, T>;
  using RulePtrT = typename RuleSeqT::RulePtr;

  EntityAttribute(const T& baseValue)
  : _baseValue(baseValue) { }

  RulePtrT addRule(RulePtrT rule) {
    _rules.add(rule);
    return rule;
  }

  template<typename R, typename ...Args>
  std::shared_ptr<R> addRule(Args&& ...args) {
    auto rule = std::make_shared<R>(std::forward<Args>(args)...);
    _rules.addRule(rule);
    rule->setup();
    return rule;
  }

  RulePtrT addRule(EntityRuleFn<E, T> callback) {
    return addRule<CallbackEntityRule<E, T>>(callback);
  }

  T calculateValue(std::shared_ptr<E> entity) {
    return _rules.calculateValue(entity, _baseValue);
  }

private:
  const T& _baseValue;
  RuleSeqT _rules;
};

template<typename E, typename T>
using EntityAttributePtr = std::shared_ptr<EntityAttribute<E, T>>;

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
