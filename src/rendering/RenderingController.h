//
//  RenderingController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef RenderingController_h
#define RenderingController_h

#include <ofEasyCam.h>
#include "Colors.h"
#include "Common.h"
#include "Params.h"
#include "PostProcController.h"
#include "State.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

class CameraParams : public Params {
public:
  CameraParams() {
    add(_spinEnabled
        .setKey("spinEnabled")
        .setName("Spin Enabled")
        .setValueAndDefault(false));
    add(_spinRate
        .setKey("spinRate")
        .setName("Spin Rate")
        .setValueAndDefault(ofVec3f(2, 4, 5))
        .setRange(ofVec3f(-10), ofVec3f(10)));
  }

  bool spinEnabled() const { return _spinEnabled.get(); }
  const ofVec3f& spinRate() const { return _spinRate.get(); }

private:
  TParam<bool> _spinEnabled;
  TParam<ofVec3f> _spinRate;
};

class FogParams : public ParamsWithEnabled {
public:
  FogParams() {
    add(_density
        .setKey("density")
        .setName("Density")
        .setValueAndDefault(0.001f)
        .setRange(0, 0.004f));
    add(_useBackgroundColor
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

  float density() const { return _density.get(); }
  bool useBackgroundColor() const { return _useBackgroundColor.get(); }

  ValueRange<float> distance;

private:
  TParam<float> _density;
  TParam<bool> _useBackgroundColor;
};

class RenderingController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(camera
          .setKey("camera")
          .setName("Camera"));
      add(fog
          .setKey("fog")
          .setName("Fog"));
      add(postProc
          .setKey("postProc")
          .setName("Post Processing"));
    }

    CameraParams camera;
    FogParams fog;
    PostProcController::Params postProc;
  };

  static shared_ptr<RenderingController> create(const Params& params, const ColorTheme& colors);

  virtual void update(const State& state) = 0;
  virtual void beginDraw(const State& state) = 0;
  virtual void endDraw(const State& state) = 0;
  virtual void resetCamera() = 0;
#ifdef ENABLE_SYPHON
  virtual void pushToSyphon(ofxSyphonServer& syphonServer) = 0;
#endif
};

#endif /* RenderingController_h */
