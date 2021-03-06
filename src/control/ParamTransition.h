//
//  ParamTransition.h
//  memory
//
//  Created by tekt on 10/14/16.
//
//

#ifndef ParamTransition_h
#define ParamTransition_h

#include <memory>
#include <vector>
#include "../control/Params.h"
#include "../core/Common.h"

class Action;
class Context;
class ParamPreset;

// Base class for transitions of parameters from some value to another.
class AbstractParamTransition
: public NonCopyable
, public Outputable
, public std::enable_shared_from_this<AbstractParamTransition> {
public:
  virtual void apply(float ratio) = 0;

  std::shared_ptr<Action>
  createApplyAction(float duration,
                    const Context& context);

  void abortApplyAction();

  const std::string& name() const { return _name; }
  void setName(std::string name) { _name = name; }
private:
  std::shared_ptr<Action> _applyAction;
  std::string _name;
};

using ParamTransitionPtr = std::shared_ptr<AbstractParamTransition>;

// A transition of a parameter value of type T from some value to
// another.
template<typename T>
class ParamTransition
: public AbstractParamTransition {
public:
  ParamTransition(TParam<T>& output,
                  T startValue,
                  T endValue)
  : _output(output)
  , _startValue(startValue)
  , _endValue(endValue) { }

  void apply(float ratio) override {
    _output.set(getInterpolated(_startValue,
                                _endValue,
                                ratio));
  }

  std::string typeName() const override { return "ParamTransition"; }

protected:
  void outputFields(std::ostream& os) const override;

private:
  TParam<T>& _output;
  T _startValue;
  T _endValue;
};

// do this down here because xcode's indentation logic is broken
template<typename T>
void ParamTransition<T>::outputFields(std::ostream& os) const {
  os << "output: (key: " << _output.getKey() << ")";
  os << ", start: " << _startValue;
  os << ", end: " << _endValue;
}

// A set of multiple parameter transitions.
class ParamTransitionSet
: public AbstractParamTransition {
public:
  using Storage = std::vector<ParamTransitionPtr>;

  void loadCurrentToPreset(Params& params,
                           const ParamPreset& preset);

  void apply(float ratio) override;

  std::string typeName() const override { return "ParamTransitionSet"; }

protected:
  void outputFields(std::ostream& os) const override;

private:
  Storage _transitions;
};

#endif /* ParamTransition_h */
