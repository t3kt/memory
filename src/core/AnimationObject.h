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
#include <ofTypes.h>
#include <ofxChoreograph.h>
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

  AnimationObject(const Params& params, const State& state);
  
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

  ExpandingSphereAnimation(ofVec3f position, const Params& params, ofFloatColor color, const State& state);

  virtual void draw(const State& state) override;

protected:
  virtual std::string typeName() const override {
    return "ExpandingSphereAnimation";
  }

private:
  const Params& _params;
  const ofFloatColor _color;
};

class AbstractAnimationSequenceFactory {
public:
  class Params : public ::Params {
  public:
    Params();
  private:
  };

  AbstractAnimationSequenceFactory(const Params& params);
protected:
  const Params& _params;
};

template<typename T>
class RampFactory {
public:
  class Params : public ::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;

    void resetToDefaults() override {
      _duration.resetToDefault();
      _startValue.resetToDefault();
      _endValue.resetToDefault();
    }

    bool hasDefaults() const override { return true; }

    float duration() const { return _duration.get(); }
    const T& startValue() const { return _startValue.get(); }
    const T& endValue() const { return _endValue.get(); }
  private:
    TParam<float> _duration;
    TParam<T> _startValue;
    TParam<T> _endValue;
  };

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
