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
#include "ConnectorRenderer.h"
#include "Context.h"
#include "ObserverPreRenderer.h"
#include "ObserverRenderer.h"
#include "OccurrenceRenderer.h"
#include "Params.h"
#include "PostProcController.h"
#include "ThresholdRenderer.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

class ObserverRenderingParams : public Params {
public:
  ObserverRenderingParams() {
    add(preRenderer
        .setKey("preRenderer")
        .setName("Pre Renderer"));
    add(renderer
        .setKey("renderer")
        .setName("Renderer"));
    add(instancedRenderer
        .setKey("instancedRenderer")
        .setName("Instanced Renderer"));
    add(connectorRenderer
        .setKey("connectorRenderer")
        .setName("Connector Renderer"));
    add(thresholdRenderer
        .setKey("thresholdRenderer")
        .setName("Threshold"));
  }

  ObserverPreRenderer::Params preRenderer;
  ObserverRenderer::Params renderer;
  InstancedObserverRenderer::Params instancedRenderer;
  ObserverObserverConnectorRenderer::Params connectorRenderer;
  AbstractThresholdRenderer::Params thresholdRenderer;
};

class OccurrenceRenderingParams : public Params {
public:
  OccurrenceRenderingParams() {
    add(renderer
        .setKey("renderer")
        .setName("Renderer"));
    add(connectorRenderer
        .setKey("connectorRenderer")
        .setName("Connector Renderer"));
    add(occurrenceConnectorRenderer
        .setKey("occurrenceConnectorRenderer")
        .setName("Occurrence Connector Renderer"));
  }

  OccurrenceRenderer::Params renderer;
  ObserverOccurrenceConnectorRenderer::Params connectorRenderer;
  OccurrenceOccurrenceConnectorRenderer::Params occurrenceConnectorRenderer;
};

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
      add(observers
          .setKey("observers")
          .setName("Observers"));
      add(occurrences
          .setKey("occurrences")
          .setName("Occurrences"));
      add(fog
          .setKey("fog")
          .setName("Fog"));
      add(postProc
          .setKey("postProc")
          .setName("Post Processing"));
    }

    CameraController::Params camera;
    ObserverRenderingParams observers;
    OccurrenceRenderingParams occurrences;
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
  void draw();
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
  std::shared_ptr<ObserverPreRenderer> _observerPreRenderer;
  std::shared_ptr<ObserverRenderer> _observerRenderer;
  std::shared_ptr<InstancedObserverRenderer> _instancedObserverRenderer;
  std::shared_ptr<ObserverObserverConnectorRenderer> _observerConnectorRenderer;
  std::shared_ptr<ObserverThresholdRenderer> _observerThresholdRenderer;
  std::shared_ptr<OccurrenceRenderer> _occurrenceRenderer;
  std::shared_ptr<ObserverOccurrenceConnectorRenderer> _observerOccurrenceConnectorRenderer;
  std::shared_ptr<OccurrenceOccurrenceConnectorRenderer> _occurrenceOccurrenceConnectorRenderer;
  std::shared_ptr<PostProcController> _postProc;
  //  ofLight _light;
};

#endif /* RenderingController_h */
