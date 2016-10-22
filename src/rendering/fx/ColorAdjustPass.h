//
//  ColorAdjustPass.h
//  memory
//
//  Created by tekt on 8/10/16.
//
//

#ifndef ColorAdjustPass_h
#define ColorAdjustPass_h

#include <ofShader.h>
#include <RenderPass.h>
#include "../../control/Params.h"

class ColorAdjustPass
: public itg::RenderPass {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(brightness
          .setKey("brightness")
          .setName("Brightness")
          .setRange(-1, 1)
          .setValueAndDefault(0));
      add(contrast
          .setKey("contrast")
          .setName("Contrast")
          .setRange(0, 4)
          .setValueAndDefault(1));
      add(hueOffset
          .setKey("hueOffset")
          .setName("Hue Offset")
          .setRange(-1, 1)
          .setValueAndDefault(0));
      add(saturationMult
          .setKey("saturationMult")
          .setName("Saturation Mult")
          .setRange(0, 3)
          .setValueAndDefault(1));
      add(valueMult
          .setKey("valueMult")
          .setName("Value Mult")
          .setRange(0, 3)
          .setValueAndDefault(1));
    }
    TParam<float> brightness;
    TParam<float> contrast;
    TParam<float> hueOffset;
    TParam<float> saturationMult;
    TParam<float> valueMult;
  };

  ColorAdjustPass(const ofVec2f& aspect,
                  bool arb);

  void setParams(const Params* params) { _params = params; }

  void render(ofFbo& readFbo,
              ofFbo& writeFbo,
              ofTexture& depth) override;
private:
  const Params* _params;
  ofShader _shader;
};

#endif /* ColorAdjustPass_h */
