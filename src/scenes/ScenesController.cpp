//
//  ScenesController.cpp
//

#include "../app/AppSystem.h"
#include "../control/CommandsController.h"
#include "../core/Context.h"
#include "../scenes/ScenesController.h"


// for test scene building
#include "../scenes/SpawnSceneNode.h"

void ScenesController::setup() {
//  // make test scene
//  auto scene = std::make_shared<Scene>();
//  scene->setName("testScene1");
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(2.0f));
//    node->setPosition(ControlValues::createValue(ofVec3f(0, 0, 0)));
//    scene->addNode(node);
//  }
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(3.0f));
//    node->setPosition(ControlValues::createValue(ofVec3f(0, 0, 0)));
//    scene->addNode(node);
//  }
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(3.1f));
//    node->setPosition(ControlValues::createValue(ofVec3f(.5, .5, 0)));
//    scene->addNode(node);
//  }
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(3.1f));
//    node->setPosition(ControlValues::createValue(ofVec3f(.6, .6, 0)));
//    scene->addNode(node);
//  }
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(3.2f));
//    node->setPosition(ControlValues::createValue(ofVec3f(.7, .7, 0)));
//    scene->addNode(node);
//  }
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(6.0f));
//    node->setPosition(ControlValues::createValue(ofVec3f(0, 0, 0)));
//    node->setVelocity(ControlValues::createValue(ofVec3f(30, 30, 30)));
//    scene->addNode(node);
//  }
//  {
//    auto node = std::make_shared<SpawnObserverSceneNode>();
//    node->setTime(ControlValues::createValue(6.0f));
//    node->setPosition(ControlValues::createValue(ofVec3f(0, 0, 0)));
//    node->setVelocity(ControlValues::createValue(ofVec3f(-30, -30, -30)));
//    scene->addNode(node);
//  }
//  scene->writeJsonTo("testScene1.json");

  AppSystem::get().commands()
  .registerCommand("loadScene", "Load Scene", [&](const CommandArgs& args) {
    if (args.hasArgType<std::string>(0)) {
      loadSceneFile(args.get<std::string>(0));
    }
    promptAndLoadScene();
    return true;
  })
  .withButton(true);
}

void ScenesController::update() {
  if (_context.activeScene) {
    if (!_context.activeScene->active()) {
      ofLogNotice() << "Finished scene '" << _context.activeScene->name() << "'...";
      _context.activeScene = nullptr;
      _context.spawningSuspended = false;
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
  auto scene = std::make_shared<Scene>();
  scene->readFromFile(filename);
  startScene(scene);
}

void ScenesController::startScene(std::shared_ptr<Scene> scene) {
  if (_context.activeScene) {
    ofLogWarning() << "Already have active scene";
    return;
  }
  ofLogNotice() << "Starting scene '" << scene->name() << "'...";
  scene->schedule(_actions);
  _context.activeScene = scene;
  _context.spawningSuspended = true;
}
