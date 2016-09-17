//
//  CameraController.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef CameraController_h
#define CameraController_h

#include <ofEasyCam.h>
#include "../app/AppActions.h"
#include "../core/Context.h"
#include "../core/Params.h"

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
: public AppActionHandler {
public:
  using Params = CameraParams;

  CameraController(Params& params, Context& context)
  : _params(params)
  , _context(context) {}

  void update();

  void applyTransform();

  ofCamera& getCamera() { return _cam; }

  bool performAction(AppAction action) override;

private:
  void resetCamera();

  Params& _params;
  Context& _context;
  ofEasyCam _cam;
  ofVec3f _rotation;
};

#endif /* CameraController_h */
