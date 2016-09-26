//
//  Common.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Common__
#define __behavior__Common__

#include <iostream>
#include <functional>
#include <memory>
#include <stdexcept>

#ifdef TARGET_OSX
#define ENABLE_SYPHON
#endif

template<typename T>
T getInterpolated(const T& a, const T& b, float amount);

class Outputable {
public:
  virtual ~Outputable() {}
  void output(std::ostream& os) const;
  virtual std::string typeName() const = 0;
protected:
  virtual void outputFields(std::ostream& os) const { }
};

std::ostream& operator<<(std::ostream& os, const Outputable& obj);

std::string ofToString(const Outputable& obj);

class NonCopyable {
public:
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  NonCopyable() {}
};

class NOT_IMPLEMENTED
: public std::runtime_error {
public:
  NOT_IMPLEMENTED(std::string description)
  : std::runtime_error("NOT IMPLEMENTED: " + description) { }
  NOT_IMPLEMENTED()
  : std::runtime_error("NOT IMPLEMENTED") { }
};

template<typename T>
class ValueOrRef {
public:
  static ValueOrRef<T> fromRef(T* val) {
    return ValueOrRef<T>(false, val);
  }

  static ValueOrRef<T> owningValue(T initialValue) {
    auto result = owningValue();
    result._value = initialValue;
    return result;
  }

  static ValueOrRef<T> owningValue() {
    ValueOrRef<T> result(true, new T());
    return result;
  }

  ~ValueOrRef() {
    if (_ownsValue) {
      delete _value;
    }
  }

  const T& get() const { return *_value; }
  void set(const T& value) { *_value = value; }

private:
  ValueOrRef(bool owns, T* val)
  : _ownsValue(owns)
  , _value(val) { }

  const bool _ownsValue;
  T* _value;
};

template<typename E>
using PtrRefAction = std::function<void(std::shared_ptr<E>&)>;

template<typename E>
using PtrAction = std::function<void(std::shared_ptr<E>)>;

template<typename E>
using PtrPredicate = std::function<bool(std::shared_ptr<E>)>;

template<typename E>
using PtrRefPredicate = std::function<bool(std::shared_ptr<E>&)>;

#endif /* defined(__behavior__Common__) */
