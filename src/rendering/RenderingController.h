//
//  RenderingController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef RenderingController_h
#define RenderingController_h

#include "Params.h"
#include "State.h"
#include "Colors.h"

class CameraParams : public Params {
public:
  CameraParams();

  bool spinEnabled() const { return _spinEnabled.get(); }
  const ofVec3f& spinRate() const { return _spinRate.get(); }

private:
  TParam<bool> _spinEnabled;
  TParam<ofVec3f> _spinRate;
};

class FogParams : public Params {
public:
  FogParams();

  bool enabled() const { return _enabled.get(); }
  float density() const { return _density.get(); }
  bool useBackgroundColor() const { return _useBackgroundColor.get(); }

  ValueRange<float> distance;

private:
  TParam<bool> _enabled;
  TParam<float> _density;
  TParam<bool> _useBackgroundColor;
};

class RenderingController {
public:
  class Params : public ::Params {
  public:
    Params();

    CameraParams camera;
    FogParams fog;
  };

  static shared_ptr<RenderingController> create(const Params& params, const ColorTheme& colors);

  virtual void update(const State& state) = 0;
  virtual void beginDraw(const State& state) = 0;
  virtual void endDraw(const State& state) = 0;
  virtual void resetCamera() = 0;
};

#endif /* RenderingController_h */
