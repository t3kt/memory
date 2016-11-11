//
//  Scenes.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include <vector>
#include "../core/JsonIO.h"

class SceneNode
: public ofxTCommon::NonCopyable
, public JsonReadable
, public JsonWritable {
public:
};

using SceneNodePtr = std::shared_ptr<SceneNode>;
using SceneNodeList = std::vector<SceneNodePtr>;

class Scene
: public ofxTCommon::NonCopyable
, public JsonReadable
, public JsonWritable {
public:
  const std::string& name() const { return _name; }
  const SceneNodeList& nodes() const { return _nodes; }

  void read_json(const Json& obj) override;
  Json to_json() const override;
private:
  std::string _name;
  SceneNodeList _nodes;
};
