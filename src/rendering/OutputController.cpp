//
//  OutputController.cpp
//

#include <ofConstants.h>
#include <ofTexture.h>
#include <ofAppGLFWWindow.h>
#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
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
  AppSystem::get().commands()
  .registerCommand("toggleFullscreen", "Toggle Fullscreen", [&](const CommandArgs& args) {
    _params.fullscreen.toggle();
    return true;
  }, true, 'f');
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
