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
    add(size
        .setKey("size")
        .setName("Size")
        .setValueAndDefault(6)
        .setRange(0, 10));
  }

  bool reflect(ofVec3f* velocity, ofVec3f* position) const override;

  ofVec3f randomPoint() const override;

  TParam<float> size;
};

#endif /* Bounds_h */
