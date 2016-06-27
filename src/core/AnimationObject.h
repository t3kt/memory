//
//  AnimationObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef AnimationObject_h
#define AnimationObject_h

#include <string>
#include <ofParameter.h>
#include "WorldObject.h"
#include "ObjectManager.h"
#include "Timing.h"
#include "State.h"
#include "Params.h"
#include "ValueSupplier.h"

class AnimationObject : public StandardWorldObject {
public:
  class Params : public ::Params {
  public:
    explicit Params(std::string name);

    ofParameter<bool> enabled;
    ofParameter<float> delay;
    ofParameter<float> duration;
  };
  
  AnimationObject(float delay, float duration);
  AnimationObject(const Params& params);
  
  virtual void draw(const State& state) override = 0;
  virtual void output(std::ostream& os) const override;
  
  void show() { _visible = true; }
  void hide() { _visible = false; }
  
  virtual bool visible() const override {
    return alive() && _visible;
  }
  
  shared_ptr<DurationAction>
  createUpdaterAction(float time, ObjectManager<AnimationObject>& animations);
protected:
  inline float percentage() const { return _percentage; }
private:
  bool _visible;
  float _delay;
  float _duration;
  float _percentage;
  
  friend class AnimationUpdater;
};

class ExpandingSphereAnimation : public AnimationObject {
public:
  class Params : public AnimationObject::Params {
  public:
    explicit Params(std::string name);

    ValueRange<float> radius;
    ValueRange<float> alpha;
    RandomHsbFloatColorSupplier color;
  };

  ExpandingSphereAnimation(ofVec3f position, const Params& params);

  virtual void draw(const State& state) override;
  virtual void output(std::ostream& os) const override;

private:
  const Params& _params;
  const ofFloatColor _color;
  ofVec3f _position;
};

#endif /* AnimationObject_h */