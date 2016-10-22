//
//  ofxTEnums.h
//

#pragma once

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include "./ofxTCommon.h"

namespace ofxTCommon {

  template<typename T>
  class EnumTypeInfo
  : public NonCopyable {
  public:
    static_assert(std::is_enum<T>::value, "T must be an enum type");

    EnumTypeInfo(std::initializer_list<std::pair<std::string, T>> entries) {
      for (std::pair<std::string, T> entry : entries) {
        _stringToEnum.insert(entry);
        _enumToString.insert(std::make_pair(entry.second, entry.first));
        _values.push_back(entry.second);
      }
    }

    bool tryParseString(const std::string& str, T* result) const {
      auto iter = _stringToEnum.find(str);
      if (iter == _stringToEnum.end()) {
        return false;
      } else {
        *result = iter->second;
        return true;
      }
    }
    bool tryToString(const T& value, std::string* result) const {
      auto iter = _enumToString.find(value);
      if (iter == _enumToString.end()) {
        return false;
      } else {
        *result = iter->second;
        return true;
      }
    }
    std::string toString(const T& value) const {
      std::string name;
      if (!tryToString(value, &name)) {
        throw std::invalid_argument("Unknown enum value");
      }
      return name;
    }
    T parseString(const std::string& str) const {
      T value;
      if (!tryParseString(str, &value)) {
        throw std::invalid_argument("Unknown enum value: " + str);
      }
      return value;
    }
    const std::vector<T>& values() const { return _values; }
  private:
    std::map<std::string, T> _stringToEnum;
    std::map<T, std::string> _enumToString;
    std::vector<T> _values;
  };

  template<typename T>
  const EnumTypeInfo<T>& getEnumInfo();

  template<typename T>
  std::string enumToString(T value) {
    return getEnumInfo<T>().toString(value);
  }

  template<typename T>
  T parseEnum(const std::string& str) {
    return getEnumInfo<T>().parseString(str);
  }
  
  template<typename T>
  const std::vector<T>& enumValues() {
    return getEnumInfo<T>().values();
  }
  
}
