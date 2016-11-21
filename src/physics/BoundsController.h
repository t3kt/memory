//
//  BoundsController.h
//

#pragma once

#include <ofColor.h>
#include <ofVec3f.h>
#include <ofxTCommon.h>
#include "../control/Params.h"
#include "../core/Component.h"

class DebugParams;

class BoundsParams
: public Params {
public:
  BoundsParams() {
    add(size
        .setKey("size")
        .setName("Size")
        .setValueAndDefault(500)
        .setRange(0, 1000));
  }

  TParam<float> size;
};

class BoundsController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using Params = BoundsParams;

  BoundsController(const Params& params,
                   DebugParams& debugParams);

  void setup() override;

  bool reflect(ofVec3f* velocity,
               ofVec3f* position) const;

  ofVec3f clampPoint(const ofVec3f& position) const;

  ofVec3f randomPoint() const;

  ofVec3f scalePoint(const ofVec3f& position) const;

  void draw() override;

private:
  const Params& _params;
  DebugParams& _debugParams;
  const ofFloatColor& _boundsColor;
};

