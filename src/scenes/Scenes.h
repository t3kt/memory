//
//  Scenes.h
//

#pragma once

#include <functional>
#include <memory>
#include <ofxTCommon.h>
#include <ofxTJsonIO.h>
#include <vector>
#include "../scenes/SceneValue.h"

class SceneNode;
using SceneNodePtr = std::shared_ptr<SceneNode>;
using SceneNodeList = std::vector<SceneNodePtr>;

using SceneNodeFactory = std::function<SceneNodePtr()>;

class SceneNode
: public ofxTCommon::NonCopyable
, public ofxTCommon::Outputable
, public ofxTCommon::JsonReadable
, public ofxTCommon::JsonWritable {
public:
  static void registerType(const std::string& typeName,
                           SceneNodeFactory factory);
  static SceneNodePtr create(const std::string& typeName);

  virtual void begin() = 0;
  virtual void end() { }

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;
protected:
  void outputFields(std::ostream& os) const override;
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
