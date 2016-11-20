//
//  Scenes.cpp
//

#include <functional>
#include <unordered_map>
#include "../scenes/Scenes.h"
#include "../scenes/SpawnSceneNode.h"

using namespace ofxTCommon;

using SceneNodeFactory = std::function<SceneNodePtr()>;

std::unordered_map<std::string, SceneNodeFactory> typeFactories {
  {SpawnObserverSceneNode::typeName(),
    []() { return std::make_shared<SpawnObserverSceneNode>(); }},
};

static SceneNodePtr createNode(const std::string &typeName) {
  auto iter = typeFactories.find(typeName);
  if (iter == typeFactories.end()) {
    ofLogWarning() << "Unrecognized scene node type: " << typeName;
    return nullptr;
  }
  return iter->second();
}

void Scene::schedule(ActionsController &actions) {
  ActionFinishCallback finishCallback = [&]() {
    _activeCount--;
  };
  for (auto& entry : _nodes) {
    _activeCount++;
    entry.second->schedule(actions, finishCallback);
  }
}

void Scene::readJson(const ofJson &obj) {
  _name = JsonUtil::fromJsonField<std::string>(obj, "name", "scene");
  _nodes.clear();
  auto iter = obj.find("nodes");
  if (iter != obj.end() && !iter.value().is_null()) {
    const auto& nodesArray = iter.value();
    JsonUtil::assertIsArray(nodesArray);
    for (const auto& nodeObj : nodesArray) {
      JsonUtil::assertIsObject(nodeObj);
      std::string typeName = nodeObj["type"];
      auto node = createNode(typeName);
      if (!node) {
        continue;
      }
      _nodes.insert(std::make_pair(node->id(), node));
    }
  }
}

ofJson Scene::toJson() const {
  auto nodesArray = ofJson::array();
  for (const auto& entry : _nodes) {
    nodesArray.push_back(entry.second->toJson());
  }
  return {
    {"name", _name},
    {"nodes", nodesArray},
  };
}
