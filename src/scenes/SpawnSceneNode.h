//
//  SpawnSceneNode.h
//  memory
//

#pragma once

#include <ofVec3f.h>
#include "../control/ControlValue.h"
#include "../scenes/Scenes.h"

class BoundsController;
class Context;

class SpawnSceneNode
: public SceneNode {
public:
  SpawnSceneNode();

  void schedule(ActionsController& actions,
                ActionFinishCallback finishCallback) override;

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;

  void setTime(ControlValue<float> time) { _time = time; }
  void setPosition(ControlValue<ofVec3f> position) {
    _position = position;
  }
  void setVelocity(ControlValue<ofVec3f> velocity) {
    _velocity = velocity;
  }
protected:
  virtual void spawn() = 0;

  const BoundsController& _bounds;
  Context& _context;

  ControlValue<float> _time;
  ControlValue<ofVec3f> _position;
  ControlValue<ofVec3f> _velocity;
};

class RateObserverSpawnerParams;

class SpawnObserverSceneNode
: public SpawnSceneNode {
public:
  static const char* typeName() { return "spawnObserver"; }

  SpawnObserverSceneNode();

  void readJson(const ofJson& obj) override;
  ofJson toJson() const override;
protected:
  void spawn() override;
private:
  ControlValue<float> _decayRate;
  const RateObserverSpawnerParams& _params;
};
