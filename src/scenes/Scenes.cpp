//
//  Scenes.cpp
//

#include <functional>
#include <unordered_map>
#include "../core/Actions.h"
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

void SceneNode::schedule(ActionsController &actions) {
  if (_beginTime.isSpecified()) {
    auto time = _beginTime.get();
    actions.addDelayed(time, [&](Context&, ActionsController&) {
      begin();
      return ActionResult::cancel();
    });
  }
  if (_endTime.isSpecified()) {
    auto time = _endTime.get();
    actions.addDelayed(time, [&](Context&, ActionsController&) {
      end();
      return ActionResult::cancel();
    });
  }
}

void SceneNode::readJson(const ofJson &obj) {
  _beginTime.readJson(obj["beginTime"]);
  _endTime.readJson(obj["endTime"]);
}

ofJson SceneNode::toJson() const {
  auto obj = ofJson::object();
  _beginTime.writeFieldTo(obj, "beginTime");
  _endTime.writeFieldTo(obj, "endTime");
  return obj;
}

void Scene::schedule(ActionsController &actions) {
  for (auto& node : _nodes) {
    node->schedule(actions);
  }
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
      auto node = createNode(typeName);
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
