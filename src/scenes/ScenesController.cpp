//
//  ScenesController.cpp
//

#include "../core/Context.h"
#include "../scenes/ScenesController.h"

void ScenesController::update() {
  if (_context.activeScene) {
    if (!_context.activeScene->active()) {
      _context.activeScene = nullptr;
    }
  }
}
