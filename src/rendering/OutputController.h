//
//  OutputController.h
//  memory
//
//  Created by tekt on 10/8/16.
//
//

#ifndef OutputController_h
#define OutputController_h

#include <memory>
#include <ofConstants.h>
#include "../app/AppActions.h"
#include "../control/Params.h"
#include "../core/Component.h"
#include "../core/Events.h"

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
        .setValueAndDefault(false));
  }

  TParam<bool> fullscreen;
  TParam<bool> externalEnabled;
};

class OutputController
: public ComponentBase
, public AppActionHandler {
public:
  using Params = OutputParams;

  OutputController(Params& params,
                   ofAppGLFWWindow& window)
  : _params(params)
  , _window(window) { }

  void setup() override;

  void pushTexture(ofTexture* texture);
  void pushScreen();

  bool performAction(AppAction action) override;

  glm::ivec2 resolution() const;

  TEvent<ValueEventArgs<glm::ivec2>> resolutionChanged;

private:
  Params& _params;
  ofAppGLFWWindow& _window;

#ifdef ENABLE_SYPHON
  ofxSyphonServer _syphonServer;
#endif
};

#endif /* OutputController_h */
