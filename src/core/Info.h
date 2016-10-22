//
//  Info.h
//  memory
//
//  Created by tekt on 8/8/16.
//
//

#ifndef Info_h
#define Info_h

#include <algorithm>
#include <ofTypes.h>
#include <ofUtils.h>
#include <ofxTextAlignTTF.h>
#include <string>
#include <utility>
#include <vector>
#include "../core/Common.h"

class Info
: public NonCopyable {
public:
  using Line = std::pair<std::string, std::string>;
  using StorageList = std::vector<Line>;
  using iterator = StorageList::iterator;
  using const_iterator = StorageList::const_iterator;

  Info()
  : _labelMaxLength(0)
  , _valueMaxLength(0) { }

  void add(const std::string& label, std::string value) {
    _labelMaxLength = std::max(_labelMaxLength, label.size());
    _valueMaxLength = std::max(_valueMaxLength, value.size());
    _lines.emplace_back(label, value);
  }

  template<typename T>
  void add(const std::string& label, const T& value) {
    add(label, ofToString(value));
  }

  void add(const std::string& label, const ofVec3f& value) {
    add(label, ofToString(value, 2));
  }
  void add(const std::string& label, const float& value) {
    add(label, ofToString(value, 2));
  }

  void resetSize() {
    _labelMaxLength = 0;
    _valueMaxLength = 0;
  }

  void clear() { _lines.clear(); }

  bool empty() const { return _lines.empty(); }

  std::size_t size() const { return _lines.size(); }

  std::size_t labelMaxLength() const { return _labelMaxLength; }
  std::size_t valueMaxLength() const { return _valueMaxLength; }

  iterator begin() { return _lines.begin(); }
  iterator end() { return _lines.end(); }
  const_iterator begin() const { return _lines.begin(); }
  const_iterator end() const { return _lines.end(); }

private:
  StorageList _lines;
  std::size_t _labelMaxLength;
  std::size_t _valueMaxLength;
};

class InfoBox {
public:
  enum class Position {
    TOP_RIGHT,
    BOTTOM_RIGHT,
  };

  InfoBox(Position position);

  void drawInfo(const Info& info);

private:
  ofRectangle getRect(const Info& info);

  Position _position;
  ofxTextAlignTTF _text;
};

#endif /* Info_h */
