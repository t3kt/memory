//
//  SpawnSceneNode.h
//  memory
//

#pragma once

#include <ofVec3f.h>
#include "../scenes/Scenes.h"
#include "../scenes/SceneValue.h"

class BoundsController;

class SpawnSceneNode
: public SceneNode {
public:
  SpawnSceneNode();

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;
protected:
  const BoundsController& _bounds;

  SceneValue<ofVec3f> _position;
  SceneValue<ofVec3f> _velocity;
};

class RateObserverSpawnerParams;

class SpawnObserverSceneNode
: public SpawnSceneNode {
public:
  static const char* typeName() { return "spawnObserver"; }

  SpawnObserverSceneNode();

  void begin() override;

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;

private:
  SceneValue<float> _decayRate;
  const RateObserverSpawnerParams& _params;
};
