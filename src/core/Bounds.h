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
  virtual bool reflect(ofVec3f* velocity, ofVec3f* position) const = 0;
  virtual ofVec3f randomPoint() const = 0;
};

class SimpleCubeBounds : public Bounds {
public:
  SimpleCubeBounds() {
    add(_size
        .setKey("size")
        .setName("Size")
        .setValueAndDefault(6)
        .setRange(0, 10));
  }

  SimpleCubeBounds& set(float sz) {
    _size.set(sz);
    return *this;
  }
  SimpleCubeBounds& setParamRange(float minVal, float maxVal) {
    _size.setMin(minVal);
    _size.setMax(maxVal);
    return *this;
  }

  bool reflect(ofVec3f* velocity, ofVec3f* position) const override;

  ofVec3f randomPoint() const override;

  float size() const { return _size.get(); }

private:
  TParam<float> _size;
};

#endif /* Bounds_h */
