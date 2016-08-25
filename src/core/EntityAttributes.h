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

enum class AttributeRuleType {
  ADD,
  MULTIPLY,
  REPLACE,
};

template<typename T>
class AttributeRule {
public:
  bool applyTo(T* value) {
    if (!test(value)) {
      return false;
    }
    modifyValue(value);
    return true;
  }
protected:
  virtual bool test(const T* value) { return true; }
  virtual void modifyValue(T* value) = 0;
};

template<typename T>
class AttributeReplaceRule
: public AttributeRule<T> {
public:
  AttributeReplaceRule(const T& arg)
  : _arg(arg) { }
protected:
  virtual void modifyValue(T* value) override {
    *value = _arg;
  }

  const T& _arg;
};

template<typename T>
class AttributeAddRule
: public AttributeRule<T> {
public:
  AttributeAddRule(const T& arg)
  : _arg(arg) { }
protected:
  virtual void modifyValue(T* value) override {
    *value += _arg;
  }

  const T& _arg;
};

template<typename T>
class AttributeMultiplyRule
: public AttributeRule<T> {
public:
  AttributeMultiplyRule(const T& arg)
  : _arg(arg) { }
protected:
  virtual void modifyValue(T* value) override {
    *value *= _arg;
  }

  const T& _arg;
};

template<typename T>
class EntityAttribute {
public:
  using AttrT = EntityAttribute<T>;

//  AttrT add(
private:
  T _value;
};

class EntityAttributes {
public:

  EntityAttribute<ofFloatColor> color;
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
