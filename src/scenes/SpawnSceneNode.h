//
//  SpawnSceneNode.h
//  memory
//

#pragma once

#include <ofVec3f.h>
#include "../scenes/Scenes.h"
#include "../scenes/SceneValue.h"

class SpawnSceneNode
: public SceneNode {
public:
private:
  SceneValue<ofVec3f> _position;
  SceneValue<ofVec3f> _velocity;
  SceneValue<float> _decayRate;
};
