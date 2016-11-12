//
//  Scenes.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <vector>

class SceneNode
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
};

using SceneNodePtr = std::shared_ptr<SceneNode>;
using SceneNodeList = std::vector<SceneNodePtr>;

class Scene
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  const std::string& name() const { return _name; }
  const SceneNodeList& nodes() const { return _nodes; }

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;
private:
  std::string _name;
  SceneNodeList _nodes;
};
