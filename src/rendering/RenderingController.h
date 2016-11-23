//
//  RenderingController.h
//

#pragma once

#include <ofAppGLFWWindow.h>
#include "../rendering/CameraController.h"
#include "../rendering/Colors.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../rendering/ConnectorRenderer.h"
#include "../rendering/ConnectionTracerRenderer.h"
#include "../core/Context.h"
#include "../rendering/FogController.h"
#include "../rendering/ObserverPreRenderer.h"
#include "../rendering/ObserverRenderer.h"
#include "../rendering/OccurrencePreRenderer.h"
#include "../rendering/OccurrenceRenderer.h"
#include "../rendering/OutputController.h"
#include "../control/Params.h"
#include "../rendering/PostProcController.h"

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
    add(connections
        .setKey("connections")
        .setName("Connections"));
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
  ConnectionTracerRendererParams connections;
  FogParams fog;
  PostProcController::Params postProc;
};

class RenderingController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using Params = RenderingParams;

  RenderingController(Params& params,
                      ofAppGLFWWindow& window,
                      Context& context);

  void setup() override;
  void update() override;
  void beginDraw();
  void draw() override;
  void endDraw();

private:

  Params& _params;
  Context& _context;
  const ColorTheme& _colors;
  ofAppGLFWWindow& _window;
  std::shared_ptr<CameraController> _camera;
  PreRendererCollection _preRenderers;
  RendererCollection _renderers;
  std::shared_ptr<FogController> _fog;
  std::shared_ptr<PostProcController> _postProc;
  std::shared_ptr<OutputController> _output;
  //  ofLight _light;
  bool _previousNormalizedTexCoords;
};

