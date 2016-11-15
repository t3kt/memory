//
//  ScenesController.cpp
//

#include "../app/AppSystem.h"
#include "../core/Context.h"
#include "../scenes/ScenesController.h"

void ScenesController::setup() {
}

void ScenesController::update() {
  if (_context.activeScene) {
    if (!_context.activeScene->active()) {
      _context.activeScene = nullptr;
    }
  }
}

void ScenesController::promptAndLoadScene() {
  if (_context.activeScene) {
    ofLogWarning() << "Already have active scene";
    return;
  }
  FileAction action = [&](ofFileDialogResult& file) {
    loadSceneFile(file.getPath());
    return true;
  };
  AppSystem::get().performFileLoadAction(action);
}

void ScenesController::loadSceneFile(const std::string &filename) {
  if (_context.activeScene) {
    ofLogWarning() << "Already have active scene";
    return;
  }
  auto scene = std::make_shared<Scene>();
  scene->readFromFile(filename);
  scene->schedule(_actions);
  _context.activeScene = scene;
}

bool ScenesController::performAction(AppAction action) {
  switch (action) {
    case AppAction::LOAD_SCENE:
      promptAndLoadScene();
      return true;
    default:
      return false;
  }
}
