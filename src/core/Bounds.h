//
//  Bounds.h
//
//  Parameters that define the boundaries of the simulated world.
//

#ifndef Bounds_h
#define Bounds_h

#include "../core/Params.h"
#include "../core/ValueSupplier.h"

class Bounds : public Params {
public:
  Bounds() {
    add(size
        .setKey("size")
        .setName("Size")
        .setValueAndDefault(500)
        .setRange(0, 1000));
  }

  bool reflect(ofVec3f* velocity, ofVec3f* position) const;

  ofVec3f randomPoint() const;

  TParam<float> size;
};

#endif /* Bounds_h */
