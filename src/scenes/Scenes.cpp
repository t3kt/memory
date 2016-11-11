//
//  Scenes.cpp
//

#include "../scenes/Scenes.h"

void Scene::read_json(const Json &obj) {
  _name = JsonUtil::fromJsonField<std::string>(obj, "name", "scene");
  // read nodes...
}
