//
//  AttractionBehavior.h
//  behavior
//
//  Created by tekt on 1/24/15.
//
//

#ifndef __behavior__AttractionBehavior__
#define __behavior__AttractionBehavior__

#include <ofParameter.h>
#include <ofParameterGroup.h>

#include "Behavior.h"

class AttractionBehaviorParams : public Behavior::Params {
public:
  AttractionBehaviorParams();
  
  ofParameter<float> minDist;
  ofParameter<float> maxDist;
  ofParameter<float> minPull;
  ofParameter<float> maxPull;
};

class SingleAttractionBehavior : public Behavior {
public:
  class Params : public AttractionBehaviorParams {
  public:
    explicit Params(std::string label);
    Params& setRange(float min, float max) {
      minDist.set(min);
      maxDist.set(max);
      return *this;
    }
    Params& setPull(float min, float max) {
      minPull.set(min);
      maxPull.set(max);
      return *this;
    }
    Params& setPosition(ofVec3f pos) {
      position.set(pos);
      return *this;
    }
    
    ofParameter<ofVec3f> position;
  };
  
  explicit SingleAttractionBehavior(const Params& params)
  : _params(params) { }
  
  void update(Entity& entity, const State& state) override;
private:
  const Params& _params;
};

//class EntityAttractionBehavior : public Behavior {
//public:
//  class Params : public AttractionBehaviorParams {
//  public:
//    explicit Params(std::string label);
//    Params& setRange(float min, float max) {
//      minDist.set(min);
//      maxDist.set(max);
//      return *this;
//    }
//    Params& setPull(float min, float max) {
//      minPull.set(min);
//      maxPull.set(max);
//      return *this;
//    }
//    Params& setLimit(int lim) {
//      limit.set(lim);
//      return *this;
//    }
//    
//    ofParameter<int> limit;
//  };
//  
//  explicit EntityAttractionBehavior(const Params& params)
//  : _params(params) { }
//  
//  void update(Entity& entity, const State& state) override;
//private:
//  const Params& _params;
//};

#endif /* defined(__behavior__AttractionBehavior__) */
