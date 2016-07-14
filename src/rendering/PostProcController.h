//
//  PostProcController.h
//  memory
//
//  Created by tekt on 7/13/16.
//
//

#ifndef PostProcController_h
#define PostProcController_h

#include <memory>
#include <ofCamera.h>
#include <ofxPostProcessing.h>
#include "Common.h"
#include "Params.h"
#include "State.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

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

class PostProcController {
public:
  using Params = PostProcParams;

  PostProcController(const Params& params)
  : _params(params) {}

  void setup();

  void update(const State& state);

  void beginDraw(ofCamera& cam);
  void endDraw(ofCamera& cam);
  
#ifdef ENABLE_SYPHON
  void pushToSyphon(ofxSyphonServer& syphonServer);
#endif

private:
  const Params& _params;
  ofxPostProcessing _postProc;
  std::shared_ptr<FxaaPass> _fxaaPass;
  std::shared_ptr<EdgePass> _edgePass;
  std::shared_ptr<DofPass> _dofPass;
  std::shared_ptr<RimHighlightingPass> _rimHighlightPass;
  std::shared_ptr<BloomPass> _bloomPass;
};

#endif /* PostProcController_h */
