//
//  ofxTCommon.h
//

#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace ofxTCommon {

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

  template<typename E>
  using PtrRefAction = std::function<void(std::shared_ptr<E>&)>;

  template<typename E>
  using PtrAction = std::function<void(std::shared_ptr<E>)>;

  template<typename E>
  using PtrPredicate = std::function<bool(std::shared_ptr<E>)>;

  template<typename E>
  using PtrRefPredicate = std::function<bool(std::shared_ptr<E>&)>;

  template<typename K, typename V>
  class MapToValueIterator {
  public:
    using MapT = std::unordered_map<K, V>;
    using InnerIterator = typename MapT::iterator;
    using IteratorT = MapToValueIterator<K, V>;

    MapToValueIterator() = default;

    MapToValueIterator(const IteratorT& other) = default;

    MapToValueIterator(InnerIterator iter)
    : _iter(iter) { }

    V& operator*() {
      return _iter->second;
    }
    V* operator->() {
      return &(_iter->second);
    }

    IteratorT& operator++() {
      ++_iter;
      return *this;
    }

    IteratorT operator++(int) {
      IteratorT i(*this);
      ++(*this);
      return i;
    }

    friend bool operator==(const IteratorT& lha,
                           const IteratorT& rha) {
      return lha._iter == rha.__iter;
    }

    friend bool operator!=(const IteratorT& lha,
                           const IteratorT& rha) {
      return lha._iter != rha._iter;
    }

  private:
    InnerIterator _iter;
  };

  template<typename K, typename V>
  class ConstMapToValueIterator {
  public:
    using MapT = std::unordered_map<K, V>;
    using InnerIterator = typename MapT::const_iterator;
    using IteratorT = ConstMapToValueIterator<K, V>;

    ConstMapToValueIterator() = default;

    ConstMapToValueIterator(const IteratorT& other) = default;

    ConstMapToValueIterator(InnerIterator iter)
    : _iter(iter) { }

    const V& operator*() const {
      return _iter->second;
    }

    const V* operator->() const {
      return &(_iter->second);
    }

    IteratorT& operator++() {
      ++_iter;
      return *this;
    }

    IteratorT operator++(int) {
      IteratorT i(*this);
      ++(*this);
      return i;
    }

    friend bool operator==(const IteratorT& lha,
                           const IteratorT& rha) {
      return lha._iter == rha.__iter;
    }
    
    friend bool operator!=(const IteratorT& lha,
                           const IteratorT& rha) {
      return lha._iter != rha._iter;
    }
    
  private:
    InnerIterator _iter;
  };

}
