//
//  BoundsController.h
//  memory
//
//  Created by tekt on 10/6/16.
//
//

#ifndef BoundsController_h
#define BoundsController_h

#include <ofColor.h>
#include <ofVec3f.h>
#include "../app/AppActions.h"
#include "../control/Params.h"
#include "../core/Common.h"
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
: public NonCopyable
, public ComponentBase
, public AppActionHandler {
public:
  using Params = BoundsParams;

  BoundsController(const Params& params,
                   DebugParams& debugParams);

  bool reflect(ofVec3f* velocity,
               ofVec3f* position) const;

  ofVec3f clampPoint(const ofVec3f& position) const;

  ofVec3f randomPoint() const;

  void draw() override;

  bool performAction(AppAction action) override;

private:
  const Params& _params;
  DebugParams& _debugParams;
  const ofFloatColor& _boundsColor;
};

#endif /* BoundsController_h */
