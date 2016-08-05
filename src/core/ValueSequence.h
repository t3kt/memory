//
//  ValueSequence.h
//  memory
//
//  Created by tekt on 8/4/16.
//
//

#ifndef ValueSequence_h
#define ValueSequence_h

#include <array>
#include <functional>
#include <ofMath.h>
#include <ofUtils.h>
#include <ofxChoreograph.h>
#include "Params.h"

template<typename T, std::size_t N>
class ValueSequence
: public Params {
public:
  template<typename P>
  using ParamArray = std::array<TParam<P>, N>;
  using SeqT = ofxChoreograph::Sequence<T>;
  using SeqRefT = ofxChoreograph::SequenceRef<T>;
  using RampT = ofxChoreograph::RampTo<T>;

  ValueSequence() {
    std::function<void()> onChange = [&]() {
      rebuildSequence();
    };
    add(startValue
        .setKey("startValue")
        .setName("Start Value"));
    startValue.changed.addVoidListener(onChange, this);
    for (std::size_t i = 0; i < N; ++i) {
      auto& lengthParam = lengths[i];
      add(lengthParam
          .setKey("length" + ofToString(i))
          .setName("Length " + ofToString(i)));
      lengthParam.changed.addVoidListener(onChange, this);
      auto& valueParam = values[i];
      add(valueParam
          .setKey("value" + ofToString(i))
          .setName("Value " + ofToString(i)));
      valueParam.changed.addVoidListener(onChange, this);
    }
    rebuildSequence();
  }

  ~ValueSequence() {
    startValue.changed.removeListeners(this);
    for (std::size_t i = 0; i < N; ++i) {
      auto& lengthParam = lengths[i];
      lengthParam.changed.removeListeners(this);
      auto& valueParam = values[i];
      valueParam.changed.removeListeners(this);
    }
  }

  TParam<T> startValue;
  ParamArray<T> values;
  ParamArray<float> lengths;

  T evaluate(float position) const {
    position = ofClamp(position, 0, _sequence->getDuration());
    return _sequence->getValue(position);
  }

private:
  void rebuildSequence() {
    using ofxChoreograph::RampTo;

    const auto& prevValue = startValue.get();
    _sequence = std::make_shared<SeqT>(prevValue);
    auto& seq = *_sequence;
    for (std::size_t i = 0; i < N; ++i) {
      const auto& valueParam = values[i];
      const auto& lengthParam = lengths[i];
      seq.template then<RampTo>(valueParam.get(),
                                lengthParam.get());
    }
  }

  SeqT _sequence;
};

#endif /* ValueSequence_h */
