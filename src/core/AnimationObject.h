//
//  AnimationObject.h
//

#pragma once

#include <ofTypes.h>
#include "../control/Params.h"
#include "../control/RampFactory.h"
#include "../control/ValueRange.h"
#include "../control/ValueSequence.h"
#include "../control/ValueSupplier.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"
#include "../core/WorldObject.h"

class AnimationObject : public ParticleObject {
public:
  static const auto type = EntityType::ANIMATION;

  static std::shared_ptr<AnimationObject> createEmpty() {
    // unsupported
    return std::shared_ptr<AnimationObject>();
  }

  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(_duration
          .setKey("duration")
          .setName("Duration")
          .setValueAndDefault(1)
          .setRange(0, 8));
      setEnabledValueAndDefault(true);
    }

    float duration() const { return _duration.get(); }

    void setDuration(float duration) { _duration.set(duration); }

  private:
    TParam<float> _duration;
  };

  AnimationObject(ofVec3f pos,
                  const Params& params,
                  const ClockState& state);

  virtual void update(const ClockState& state) override;
  virtual void draw(const ClockState& state) = 0;
  
  void show() { _visible = true; }
  void hide() { _visible = false; }
  
  virtual bool visible() const override {
    return alive() && _visible;
  }

  EntityType entityType() const override { return EntityType::ANIMATION; }
  virtual std::string typeName() const override {
    return "Animation";
  }

protected:
  inline float percentage() const { return _percentage; }

private:
  float _startTime;
  bool _visible;
  float _duration;
  float _percentage;
};

class ExpandingSphereAnimation : public AnimationObject {
public:
  class Params : public AnimationObject::Params {
  public:
    Params() {
      add(radius
          .setKey("radius")
          .setName("Radius")
          .setValueRanges(0, 100)
          .setLengthRanges(0, 1));
      radius.startValue.setValueAndDefault(0);
      radius.values[0].setValueAndDefault(20);
      radius.lengths[0].setValueAndDefault(1);
      add(alpha
          .setKey("alpha")
          .setName("Alpha")
          .setValueRanges(0, 1)
          .setLengthRanges(0, 1));
      alpha.startValue.setValueAndDefault(0);
      alpha.values[0].setValueAndDefault(1);
      alpha.lengths[0].setValueAndDefault(1);
    }

    ValueSequence<float, 1> radius;
    ValueSequence<float, 1> alpha;
  };

  ExpandingSphereAnimation(ofVec3f position,
                           const Params& params,
                           const ofFloatColor& color,
                           const ClockState& state);

  virtual void draw(const ClockState& state) override;

  virtual std::string typeName() const override {
    return "ExpandingSphereAnimation";
  }

private:
  const Params& _params;
  const ofFloatColor& _color;
};

