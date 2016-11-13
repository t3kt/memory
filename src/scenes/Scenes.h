//
//  Scenes.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <vector>
#include "../scenes/SceneValue.h"

class SceneNode;
using SceneNodePtr = std::shared_ptr<SceneNode>;
using SceneNodeList = std::vector<SceneNodePtr>;

class SceneNode
: public ofxTCommon::NonCopyable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  virtual void begin() = 0;
  virtual void end() { }

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;
private:
  SceneValue<float> _beginTime;
  SceneValue<float> _endTime;
};

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
