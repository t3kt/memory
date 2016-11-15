//
//  Scenes.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <unordered_map>
#include "../core/Actions.h"
#include "../core/ObjectId.h"

class SceneNode;
using SceneNodePtr = std::shared_ptr<SceneNode>;
using SceneNodeMap = std::unordered_map<ObjectId, SceneNodePtr>;

class SceneNode
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  SceneNode()
  : _id(ObjectIds::next()) { }

  const ObjectId& id() const { return _id; }

  virtual void schedule(ActionsController& actions,
                        ActionFinishCallback finishCallback) = 0;
private:
  const ObjectId _id;
};

class Scene
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  void addNode(SceneNodePtr node) {
    _nodes.insert(std::make_pair(node->id(), node));
  }
  void setName(const std::string& name) { _name = name; }

  const std::string& name() const { return _name; }
  const SceneNodeMap& nodes() const { return _nodes; }

  void schedule(ActionsController& actions);

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;

  bool active() const { return _activeCount > 0; }
private:
  std::string _name;
  SceneNodeMap _nodes;
  int _activeCount;
};
