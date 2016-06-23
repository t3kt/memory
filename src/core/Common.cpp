//
//  Common.cpp
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#include "Common.h"

#include <ofMath.h>

ofVec3f createSignedNoiseVec3f(const ofVec3f& position) {
  return ofVec3f(ofSignedNoise(position.x),
                 ofSignedNoise(position.y),
                 ofSignedNoise(position.z));
}

ofVec3f createRandomVec3f(const ofVec3f& mins,
                          const ofVec3f& maxs) {
  return ofVec3f(ofRandom(mins.x, maxs.x),
                 ofRandom(mins.y, maxs.y),
                 ofRandom(mins.z, maxs.z));
}
ofVec3f createRandomVec3f(float max) {
  return ofVec3f(ofRandom(max),
                 ofRandom(max),
                 ofRandom(max));
}
ofVec3f wrapVec(ofVec3f vec, float min, float max) {
  vec.x = ofWrap(vec.x, min, max);
  vec.y = ofWrap(vec.y, min, max);
  vec.z = ofWrap(vec.z, min, max);
  return vec;
}


std::ostream& operator<<(std::ostream& os, const Outputable& obj) {
  obj.output(os);
  return os;
}
