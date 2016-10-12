//
//  FogController.h
//  memory
//
//  Created by tekt on 10/8/16.
//
//

#ifndef FogController_h
#define FogController_h

#include "../control/Params.h"
#include "../core/Component.h"

class ColorTheme;

class FogParams : public ParamsWithEnabled {
public:
  FogParams() {
    add(density
        .setKey("density")
        .setName("Density")
        .setValueAndDefault(0.001f)
        .setRange(0, 0.004f));
    add(useBackgroundColor
        .setKey("useBackgroundColor")
        .setName("Use Background Color")
        .setValueAndDefault(true));
    add(distance
        .setKey("distance")
        .setName("Distance")
        .setParamValuesAndDefaults(0.3, 0.9)
        .setParamRanges(0, 8));
    setEnabledValueAndDefault(true);
  }

  ValueRange<float> distance;
  TParam<float> density;
  TParam<bool> useBackgroundColor;
};

class FogController
: public ComponentBase {
public:
  using Params = FogParams;

  FogController(Params& params);

  void beginDraw();
  void endDraw();

private:
  Params& _params;
  const ColorTheme& _colors;
};

#endif /* FogController_h */
