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

class EdgePassParams : public ParamsWithEnabled {
public:
  EdgePassParams() {
    add(_hue
        .setKey("hue")
        .setName("Hue")
        .setValueAndDefault(0.5)
        .setRange(0, 1));
    add(_saturation
        .setKey("saturation")
        .setName("Saturation")
        .setValueAndDefault(0.5)
        .setRange(0, 1));
    setEnabledValueAndDefault(false);
  }

  float hue() const { return _hue.get(); }
  float saturation() const { return _saturation.get(); }
private:
  TParam<float> _hue;
  TParam<float> _saturation;
};

class DofPassParams : public ParamsWithEnabled {
public:
  DofPassParams() {
    add(_focus
        .setKey("focus")
        .setName("Focus")
        .setValueAndDefault(0.985)
        .setRange(0.9, 1.1));
    add(_aperture
        .setKey("aperture")
        .setName("Aperture")
        .setValueAndDefault(0.8)
        .setRange(0, 1.2));
    add(_maxBlur
        .setKey("maxBlur")
        .setName("Max Blur")
        .setValueAndDefault(0.6)
        .setRange(0, 1));
    setEnabledValueAndDefault(false);
  }

  float focus() const { return _focus.get(); }
  float aperture() const { return _aperture.get(); }
  float maxBlur() const { return _maxBlur.get(); }
private:
  TParam<float> _focus;
  TParam<float> _aperture;
  TParam<float> _maxBlur;
};

class PostProcParams : public ParamsWithEnabled {
public:
  PostProcParams() {
    add(fxaa
        .setKey("fxaa")
        .setName("FXAA"));
    add(edge
        .setKey("edge")
        .setName("Edge"));
    add(dof
        .setKey("dof")
        .setName("Depth of Field"));
    add(rimHighlight
        .setKey("rimHighlight")
        .setName("Rim Highlight"));
    add(bloom
        .setKey("bloom")
        .setName("Bloom"));
    setEnabledValueAndDefault(true);
    fxaa.setEnabledValueAndDefault(true);
    rimHighlight.setEnabledValueAndDefault(false);
    bloom.setEnabledValueAndDefault(true);
  }

  ParamsWithEnabled fxaa;
  EdgePassParams edge;
  DofPassParams dof;
  ParamsWithEnabled rimHighlight;
  ParamsWithEnabled bloom;
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
    PostProcParams postProc;
  };

  static shared_ptr<RenderingController> create(const Params& params, const ColorTheme& colors);

  virtual void update(const State& state) = 0;
  virtual void beginDraw(const State& state) = 0;
  virtual void endDraw(const State& state) = 0;
  virtual void resetCamera() = 0;
};

#endif /* RenderingController_h */
