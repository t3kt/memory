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

class AnimationObject : public WorldObject {
public:
  class Params : public ::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;

    bool enabled() const { return _enabled.get(); }
    float delay() const { return _delay.get(); }
    float duration() const { return _duration.get(); }

    void setEnabled(bool enabled) { _enabled.set(enabled); }
    void setDelay(float delay) { _delay.set(delay); }
    void setDuration(float duration) { _duration.set(duration); }

  private:
    TParam<bool> _enabled;
    TParam<float> _delay;
    TParam<float> _duration;
  };

  AnimationObject(const Params& params);
  
  void show() { _visible = true; }
  void hide() { _visible = false; }
  
  virtual bool visible() const override {
    return alive() && _visible;
  }
  
  shared_ptr<DurationAction>
  createUpdaterAction(float time, ObjectManager<AnimationObject>& animations);
protected:
  virtual std::string typeName() const override {
    return "Animation";
  }
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
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;

    FloatValueRange radius;
    FloatValueRange alpha;
  };

  ExpandingSphereAnimation(ofVec3f position, const Params& params, ofFloatColor color);

  virtual void draw(const State& state) override;

protected:
  virtual std::string typeName() const override {
    return "ExpandingSphereAnimation";
  }

private:
  const Params& _params;
  const ofFloatColor _color;
};

#endif /* AnimationObject_h */
