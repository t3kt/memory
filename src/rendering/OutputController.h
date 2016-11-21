//
//  OutputController.h
//

#pragma once

#include <memory>
#include <ofConstants.h>
#include <ofxTCommon.h>
#include <ofxTEvents.h>
#include "../control/Params.h"
#include "../core/Component.h"

#ifdef TARGET_OSX
#define ENABLE_SYPHON
#endif

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

class ofTexture;
class ofAppGLFWWindow;

class OutputParams : public Params {
public:
  OutputParams() {
    add(fullscreen
        .setKey("fullscreen")
        .setName("Fullscreen"));
    add(externalEnabled
        .setKey("externalEnabled")
        .setName("Enable External Send")
        .setValueAndDefault(false)
        .setSupportsPresets(false));
  }

  TParam<bool> fullscreen;
  TParam<bool> externalEnabled;
};

class OutputController
: public ComponentBase
, public ofxTCommon::NonCopyable {
public:
  using Params = OutputParams;

  OutputController(Params& params,
                   ofAppGLFWWindow& window)
  : _params(params)
  , _window(window) { }

  void setup() override;

  void pushTexture(ofTexture* texture);
  void pushScreen();

  glm::ivec2 resolution() const;

  ofxTCommon::TEvent<ofxTCommon::ValueEventArgs<glm::ivec2>> resolutionChanged;

private:
  Params& _params;
  ofAppGLFWWindow& _window;

#ifdef ENABLE_SYPHON
  ofxSyphonServer _syphonServer;
#endif
};

