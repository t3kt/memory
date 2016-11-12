//
//  Scenes.cpp
//

#include "../scenes/Scenes.h"

using namespace ofxTCommon;

void Scene::readJson(const ofJson &obj) {
  _name = JsonUtil::fromJsonField<std::string>(obj, "name", "scene");
  // read nodes...
}
