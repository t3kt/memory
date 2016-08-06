//
//  RenderingController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef RenderingController_h
#define RenderingController_h

#include <ofAppGLFWWindow.h>
#include "AppActions.h"
#include "CameraController.h"
#include "Colors.h"
#include "Common.h"
#include "Context.h"
#include "Params.h"
#include "PostProcController.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

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

class RenderingController
: public AppActionHandler {
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

    CameraController::Params camera;
    FogParams fog;
    PostProcController::Params postProc;
  };

  RenderingController(Params& params,
                      ofAppGLFWWindow& window,
                      Context& context);

  ofCamera& getCamera() { return _camera->getCamera(); }

  void setup();
  void update();
  void beginDraw();
  void endDraw();
  void updateResolution();

  bool performAction(AppAction action) override;

#ifdef ENABLE_SYPHON
  void pushToSyphon(ofxSyphonServer& syphonServer);
#endif

private:
  void beginFog();
  void endFog();

  Params& _params;
  Context& _context;
  const ColorTheme& _colors;
  const ofFloatColor& _backgroundColor;
  const ofFloatColor& _fogColor;
  ofAppGLFWWindow& _window;
  std::shared_ptr<CameraController> _camera;
  std::shared_ptr<PostProcController> _postProc;
  //  ofLight _light;
};

#endif /* RenderingController_h */
