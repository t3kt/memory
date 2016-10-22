//
//  EntityRule.h
//  memory
//
//  Created by tekt on 9/5/16.
//
//

#ifndef EntityRule_h
#define EntityRule_h

#include <functional>
#include <memory>
#include <vector>
#include "../core/Common.h"

template<typename E, typename T>
using EntityRuleFn
= std::function<T(const T&, std::shared_ptr<E>&)>;

template<typename E, typename T>
class EntityRule
: public NonCopyable {
public:
  virtual void setup() { }
  virtual T calculateValue(const T& prevValue,
                           std::shared_ptr<E>& entity) = 0;
};

template<typename E, typename T>
class CallbackEntityRule
: public EntityRule<E, T> {
public:
  CallbackEntityRule(EntityRuleFn<E, T> callback)
  : _callback(callback) { }

  T calculateValue(const T& prevValue,
                   std::shared_ptr<E>& entity) override {
    return _callback(prevValue, entity);
  }
private:
  EntityRuleFn<E, T> _callback;
};

template<typename E, typename T>
class EntityRuleSequence
: public EntityRule<E, T> {
public:
  using RulePtr = std::shared_ptr<EntityRule<E, T>>;
  using Storage = std::vector<RulePtr>;

  void addRule(RulePtr rule) {
    _rules.push_back(rule);
  }

  T calculateValue(const T& prevValue,
                   std::shared_ptr<E>& entity) override {
    T value = prevValue;
    for (auto& rule : _rules) {
      value = rule->calculateValue(value, entity);
    }
    return value;
  }

private:
  Storage _rules;
};

#endif /* EntityRule_h */
