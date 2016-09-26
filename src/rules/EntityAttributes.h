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

  EntityAttribute() { }

  AttrT& setBase(const T& baseValue) {
    _baseValue = &baseValue;
    return *this;
  }

  AttrT& addRule(RulePtrT rule) {
    _rules.add(rule);
    return *this;
  }

  template<typename R, typename ...Args>
  std::shared_ptr<R> addRule(Args&& ...args) {
    auto rule = std::make_shared<R>(std::forward<Args>(args)...);
    _rules.push_back(rule);
    rule->setup();
    return rule;
  }

  T calculateValue(std::shared_ptr<E> entity) {
    T baseValue;
    if (_baseValue != nullptr) {
      baseValue = *_baseValue;
    }
    return _rules.calculateValue(entity, baseValue);
  }

private:
  const T* _baseValue;
  RuleSeqT _rules;
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
