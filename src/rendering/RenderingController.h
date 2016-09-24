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
#include "../app/AppActions.h"
#include "../rendering/CameraController.h"
#include "../rendering/Colors.h"
#include "../core/Common.h"
#include "../rendering/ConnectorRenderer.h"
#include "../core/Context.h"
#include "../rendering/ObserverPreRenderer.h"
#include "../rendering/ObserverRenderer.h"
#include "../rendering/OccurrencePreRenderer.h"
#include "../rendering/OccurrenceRenderer.h"
#include "../core/Params.h"
#include "../rendering/PostProcController.h"

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
  }

  ObserverPreRenderer::Params preRenderer;
  ObserverRenderer::Params renderer;
  InstancedObserverRenderer::Params instancedRenderer;
  ObserverObserverConnectorRenderer::Params connectorRenderer;
};

class OccurrenceRenderingParams : public Params {
public:
  OccurrenceRenderingParams() {
    add(preRenderer
        .setKey("preRenderer")
        .setName("Pre Renderer"));
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

  OccurrencePreRenderer::Params preRenderer;
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


class RenderingParams
: public Params {
public:
  RenderingParams() {
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

class RenderingController
: public AppActionHandler {
public:
  using Params = RenderingParams;

  RenderingController(Params& params,
                      ofAppGLFWWindow& window,
                      Context& context);

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
  ofAppGLFWWindow& _window;
  std::shared_ptr<CameraController> _camera;
  PreRendererCollection _preRenderers;
  RendererCollection _renderers;
  std::shared_ptr<PostProcController> _postProc;
  //  ofLight _light;
};

#endif /* RenderingController_h */
