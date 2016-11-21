//
//  CameraController.h
//

#pragma once

#include <ofEasyCam.h>
#include <ofxTCommon.h>
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Context.h"

class CameraParams
: public Params {
public:
  CameraParams() {
    add(spinEnabled
        .setKey("spinEnabled")
        .setName("Spin Enabled")
        .setValueAndDefault(false));
    add(spinRate
        .setKey("spinRate")
        .setName("Spin Rate")
        .setValueAndDefault(ofVec3f(2, 4, 5))
        .setRange(ofVec3f(-10), ofVec3f(10)));
  }

  TParam<bool> spinEnabled;
  TParam<ofVec3f> spinRate;
};

class CameraController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using Params = CameraParams;

  CameraController(Params& params, Context& context)
  : _params(params)
  , _context(context) {}

  void setup() override;
  void update() override;

  void applyTransform();

  ofCamera& getCamera() { return _cam; }

private:
  void resetCamera();

  Params& _params;
  Context& _context;
  ofEasyCam _cam;
  ofVec3f _rotation;
};

