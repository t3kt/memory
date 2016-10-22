//
//  ofxTCommon.cpp
//
//

#include <cmath>
#include <ofMath.h>
#include <ofTypes.h>
#include <ofVec2f.h>
#include <ofVec3f.h>
#include <sstream>
#include <string>
#include "./ofxTCommon.h"

namespace ofxTCommon {

  void Outputable::output(std::ostream &os) const {
    os << typeName() << "{";
    outputFields(os);
    os << "}";
  }

  std::string ofToString(const Outputable& obj) {
    std::ostringstream os;
    obj.output(os);
    return os.str();
  }

  template<>
  ofVec2f getInterpolated(const ofVec2f& a, const ofVec2f& b, float amount) {
    return a.getInterpolated(b, amount);
  }
  template<>
  ofVec3f getInterpolated(const ofVec3f& a, const ofVec3f& b, float amount) {
    return a.getInterpolated(b, amount);
  }
  template<>
  ofFloatColor getInterpolated(const ofFloatColor& a, const ofFloatColor& b, float amount) {
    return a.getLerped(b, amount);
  }
  template<>
  float getInterpolated(const float& a, const float& b, float amount) {
    return ofLerp(a, b, amount);
  }

  template<>
  int getInterpolated(const int& a, const int& b, float amount) {
    return static_cast<int>(std::round(ofLerp(a, b, amount)));
  }

  std::ostream& operator<<(std::ostream& os, const Outputable& obj) {
    obj.output(os);
    return os;
  }

}
