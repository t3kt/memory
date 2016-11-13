//
//  SpawnSceneNode.cpp
//

#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../core/Context.h"
#include "../physics/BoundsController.h"
#include "../scenes/SpawnSceneNode.h"

using namespace ofxTCommon;

static ofVec3f randomPoint() {
  return ofVec3f(ofRandom(-1, 1),
                 ofRandom(-1, 1),
                 ofRandom(-1, 1));
}

static ofVec3f randomUnsignedPoint() {
  return ofVec3f(ofRandom(0, 1),
                 ofRandom(0, 1),
                 ofRandom(0, 1));
}

SpawnSceneNode::SpawnSceneNode()
: _bounds(AppSystem::get().simulation().bounds()) { }

void SpawnSceneNode::readJson(const ofJson &obj) {
  SceneNode::readJson(obj);
  _position.readJson(obj["position"]);
  _velocity.readJson(obj["velocity"]);
}

ofJson SpawnSceneNode::toJson() const {
  auto obj = SceneNode::toJson();
  _position.writeFieldTo(obj, "position");
  _velocity.writeFieldTo(obj, "velocity");
}

SpawnObserverSceneNode::SpawnObserverSceneNode()
: _params(AppSystem::get().params().observers.rateSpawner) { }

void SpawnObserverSceneNode::readJson(const ofJson &obj) {
  SpawnSceneNode::readJson(obj);
  _decayRate.readJson(obj["decayRate"]);
}

ofJson SpawnObserverSceneNode::toJson() const {
  auto obj = SpawnSceneNode::toJson();
  _decayRate.writeFieldTo(obj, "decayRate");
}

void SpawnObserverSceneNode::begin() {
  auto pos = _bounds.scalePoint(_position.get([]() { return randomPoint(); }));
  auto velocity = _velocity.get(ofVec3f(0, 0, 0));
//  auto decayRate = _decayRate.get(0.)
//  auto entity = std::make_shared
//  AppSystem::get().context().observers.add
  //...
}
