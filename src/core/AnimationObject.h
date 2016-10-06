//
//  AnimationObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef AnimationObject_h
#define AnimationObject_h

#include <ofTypes.h>
#include <ofxChoreograph.h>
#include <string>
#include "../core/Params.h"
#include "../core/State.h"
#include "../core/ValueSupplier.h"
#include "../core/WorldObject.h"

class AnimationObject : public WorldObject {
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

  AnimationObject(const Params& params, const State& state);

  virtual void update(const State& state) override;
  virtual void draw(const State& state) = 0;

  const ofVec3f& position() const override { return _position; }
  
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
  void outputFields(std::ostream& os) const override;

  inline float percentage() const { return _percentage; }

  ofVec3f _position;
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
          .setParamNames("Start", "End")
          .setParamRanges(0, 0.4)
          .setParamValuesAndDefaults(0, 0.2));
      add(alpha
          .setKey("alpha")
          .setName("Alpha")
          .setParamNames("Start", "End")
          .setParamRanges(0, 1)
          .setParamValuesAndDefaults(0, 1));
    }

    FloatValueRange radius;
    FloatValueRange alpha;
  };

  ExpandingSphereAnimation(ofVec3f position,
                           const Params& params,
                           const ofFloatColor& color,
                           const State& state);

  virtual void draw(const State& state) override;

  virtual std::string typeName() const override {
    return "ExpandingSphereAnimation";
  }

private:
  const Params& _params;
  const ofFloatColor& _color;
};

template<typename T>
class RampFactoryParams
: public Params {
public:
  RampFactoryParams() {
    add(_duration
        .setKey("duration")
        .setName("Duration")
        .setRange(0, 4)
        .setValueAndDefault(1));
    add(_startValue
        .setKey("startValue")
        .setName("Start Value")
        .setRange(0, 1)
        .setValueAndDefault(0));
    add(_endValue
        .setKey("endValue")
        .setName("End Value")
        .setRange(0, 1)
        .setValueAndDefault(1));
  }

  float duration() const { return _duration.get(); }
  const T& startValue() const { return _startValue.get(); }
  const T& endValue() const { return _endValue.get(); }
private:
  TParam<float> _duration;
  TParam<T> _startValue;
  TParam<T> _endValue;
};

template<typename T>
class RampFactory {
public:
  using Params = RampFactoryParams<T>;

  RampFactory(const Params& params)
  : _params(params)
  , _lastParams(params) { }

  void update(const State& state) {
    ParamVals newParams(_params);
    if (newParams != _lastParams) {
      _phrase.reset();
      _lastParams = newParams;
    }
  }

  ofxChoreograph::PhraseRef<T> getPhrase() {
    if (!_phrase) {
      rebuildPhrase();
    }
    return _phrase;
  }

private:

  class ParamVals {
  public:
    ParamVals() {}
    explicit ParamVals(const Params& params)
    : duration(params.duration())
    , startValue(params.startValue())
    , endValue(params.endValue()) { }

    float duration;
    T startValue;
    T endValue;

    friend bool operator==(const ParamVals& rhs, const ParamVals& lhs) {
      return rhs.duration == lhs.duration && rhs.startValue == lhs.startValue && rhs.endValue == lhs.endValue;
    }

    friend bool operator!=(const ParamVals& rhs, const ParamVals& lhs) {
      return !(rhs == lhs);
    }
  };

  void rebuildPhrase() {
    _phrase.reset(new ofxChoreograph::RampTo<T>(_lastParams.duration,
                                                _lastParams.startValue,
                                                _lastParams.endValue));
  }

  const Params& _params;
  ParamVals _lastParams;
  ofxChoreograph::PhraseRef<T> _phrase;
};

#endif /* AnimationObject_h */
