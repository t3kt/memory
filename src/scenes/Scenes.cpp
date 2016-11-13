//
//  Scenes.cpp
//

#include <stdexcept>
#include <unordered_map>
#include "../scenes/Scenes.h"
#include "../scenes/SpawnSceneNode.h"

using namespace ofxTCommon;

std::unordered_map<std::string, SceneNodeFactory> typeFactories {
//  {"spawnObserver",},
};


/* static */
void SceneNode::registerType(const std::string &typeName,
                             SceneNodeFactory factory) {
  typeFactories[typeName] = factory;
}

/* static */
SceneNodePtr SceneNode::create(const std::string &typeName) {
  auto iter = typeFactories.find(typeName);
  if (iter == typeFactories.end()) {
    // log...?
//    throw std::invalid_argument("Unsupported scene node type: " + typeName);
    ofLogWarning() << "Unrecognized scene node type: " << typeName;
    return nullptr;
  }
  return iter->second();
}

void SceneNode::readJson(const ofJson &obj) {
  _beginTime.readJson(obj["beginTime"]);
  _endTime.readJson(obj["endTime"]);
}

ofJson SceneNode::toJson() const {
  return {
    {"type", typeName()},
    {"beginTime", _beginTime.toJson()},
    {"endTime", _endTime.toJson()},
  };
}

void Scene::readJson(const ofJson &obj) {
  _name = JsonUtil::fromJsonField<std::string>(obj, "name", "scene");
  _nodes.clear();
  const auto& nodesArray = obj["nodes"];
  if (!nodesArray.is_null()) {
    JsonUtil::assertIsArray(nodesArray);
    for (const auto& nodeObj : nodesArray) {
      JsonUtil::assertIsObject(nodeObj);
      std::string typeName = nodeObj["type"];
      auto node = SceneNode::create(typeName);
      if (!node) {
        continue;
      }
      _nodes.push_back(node);
    }
  }
}

ofJson Scene::toJson() const {
  auto nodesArray = ofJson::array();
  for (const auto& node : _nodes) {
    nodesArray.push_back(node->toJson());
  }
  return {
    {"name", _name},
    {"nodes", nodesArray},
  };
}
