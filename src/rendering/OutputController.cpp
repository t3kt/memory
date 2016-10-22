//
//  OutputController.cpp
//  memory
//
//  Created by tekt on 10/8/16.
//
//

#include <ofTexture.h>
#include <ofAppGLFWWindow.h>
#include "../rendering/OutputController.h"

glm::ivec2 OutputController::resolution() const {
  return glm::ivec2(_window.getWidth(),
                    _window.getHeight());
}

void OutputController::setup() {
  _params.fullscreen.changed += [&](bool& fullscreen) {
    _window.setFullscreen(fullscreen);
    auto res = resolution();
    auto e = ValueEventArgs<glm::ivec2>(res);
    resolutionChanged(e);
  };
#ifdef ENABLE_SYPHON
  _syphonServer.setName("Memory Main Output");
#endif
}

void OutputController::pushTexture(ofTexture *texture) {
  //...
  if (_params.externalEnabled.get()) {
#ifdef ENABLE_SYPHON
    _syphonServer.publishTexture(texture);
#else
    //...
#endif
  }
}

void OutputController::pushScreen() {
  //...
  if (_params.externalEnabled.get()) {
#ifdef ENABLE_SYPHON
    _syphonServer.publishScreen();
#else
    //...
#endif
  }
}

bool OutputController::performAction(AppAction action) {
  switch (action) {
    case AppAction::TOGGLE_FULLSCREEN:
      _params.fullscreen.toggle();
      return true;
    default:
      return false;
  }
}
