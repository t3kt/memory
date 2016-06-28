//
//  Bounds.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#ifndef Bounds_h
#define Bounds_h

#include "Params.h"
#include "ValueSupplier.h"

class Bounds : public Params {
public:
  Bounds(std::string name);

  virtual bool reflect(ofVec3f* velocity, ofVec3f* position) const = 0;
  virtual ofVec3f randomPoint() const = 0;
};

class SimpleCubeBounds : public Bounds {
public:
  SimpleCubeBounds(std::string name);

  SimpleCubeBounds& set(float sz) {
    size.set(sz);
    return *this;
  }
  SimpleCubeBounds& setParamRange(float minVal, float maxVal) {
    size.setMin(minVal);
    size.setMax(maxVal);
    return *this;
  }

  bool reflect(ofVec3f* velocity, ofVec3f* position) const override;

  ofVec3f randomPoint() const override;

  ofParameter<float> size;
};

#endif /* Bounds_h */
