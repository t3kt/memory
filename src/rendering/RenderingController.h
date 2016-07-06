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

class RenderPassParams : public Params {
public:
  RenderPassParams()
  : Params() {
    add(_enabled
        .setKey("enabled")
        .setName("Enabled")
        .setValueAndDefault(true));
  }

  bool enabled() const { return _enabled.get(); }
  void setEnabled(bool enabled) { _enabled.set(enabled); }
protected:
  void setDefaultEnabled(bool enabled) {
    _enabled.setValueAndDefault(enabled);
  }
private:
  TParam<bool> _enabled;
};

class EdgePassParams : public RenderPassParams {
public:
  EdgePassParams()
  : RenderPassParams() {
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
    setDefaultEnabled(false);
  }

  float hue() const { return _hue.get(); }
  float saturation() const { return _saturation.get(); }
private:
  TParam<float> _hue;
  TParam<float> _saturation;
};

class DofPassParams : public RenderPassParams {
public:
  DofPassParams()
  : RenderPassParams() {
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
  }

  float focus() const { return _focus.get(); }
  float aperture() const { return _aperture.get(); }
  float maxBlur() const { return _maxBlur.get(); }
private:
  TParam<float> _focus;
  TParam<float> _aperture;
  TParam<float> _maxBlur;
};

class PostProcParams : public RenderPassParams {
public:
  PostProcParams()
  : RenderPassParams() {
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
  }

  RenderPassParams fxaa;
  EdgePassParams edge;
  DofPassParams dof;
  RenderPassParams rimHighlight;
  RenderPassParams bloom;
};

class RenderingController {
public:
  class Params : public ::Params {
  public:
    Params();

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
