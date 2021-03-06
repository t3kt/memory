//
//  ParamTransition.cpp
//  memory
//
//  Created by tekt on 10/14/16.
//
//

#include <stdexcept>
#include "../app/AppSystem.h"
#include "../control/ParamPresets.h"
#include "../control/ParamTransition.h"
#include "../core/Actions.h"
#include "../core/Context.h"
#include "../core/State.h"

static inline Logger& getLog() {
  return AppSystem::get().log().control();
}

class ApplyParamTransitionAction
: public Action {
public:
  ApplyParamTransitionAction(ParamTransitionPtr transition,
                             float duration,
                             const Context& context)
  : _transition(transition)
  , _duration(duration)
  , _age(context.rootState) { }

  ActionResult operator()(Context& context,
                          ActionsController& controller) override {
    if (_aborted) {
      getLog().logNotice("Param transition aborted");
      return ActionResult::abort();
    }
    auto age = _age.get();
    if (age > _duration) {
      getLog().logNotice("Finished param transition");
      return ActionResult::cancel();
    }
    auto ratio = age / _duration;
    _transition->apply(ratio);
    return ActionResult::continuous();
  }
private:
  ParamTransitionPtr _transition;
  AgeTracker _age;
  float _duration;
};

void ParamTransitionSet::apply(float ratio) {
  for (auto& transition : _transitions) {
    transition->apply(ratio);
  }
}

std::shared_ptr<Action>
AbstractParamTransition::createApplyAction(float duration,
                                           const Context &context) {
  if (_applyAction) {
    throw std::logic_error("Param transition already has apply action");
  }
  _applyAction.reset(new ApplyParamTransitionAction(shared_from_this(), duration, context));
  return _applyAction;
}

void AbstractParamTransition::abortApplyAction() {
  if (!_applyAction) {
    throw std::logic_error("Param transition does not have apply action to abort");
  }
  AppSystem::get().log().app().logNotice("Aborting param transition apply action");
  _applyAction->abort();
  _applyAction.reset();
}

template<typename T>
static ParamTransitionPtr
createTypedTransition(TParamBase& param, const ofJson& endJsonVal) {
  TParam<T>& typedParam = dynamic_cast<TParam<T>&>(param);
  auto endVal = ofxTCommon::JsonUtil::fromJson<T>(endJsonVal);
  auto trans = new ParamTransition<T>(typedParam,
                                      typedParam.get(),
                                      endVal);
  return std::shared_ptr<AbstractParamTransition>(trans);
}

static ParamTransitionPtr
createTransition(TParamBase& param, const ofJson& endJsonVal) {
  if (param.isGroup()) {
    getLog().logWarning("Cannot transition param group " + param.getKey());
    return nullptr;
  }
  const auto& type = param.getTypeInfo();
  if (type == typeid(float)) {
    return createTypedTransition<float>(param, endJsonVal);
  } else if (type == typeid(int)) {
    return createTypedTransition<int>(param, endJsonVal);
  } else if (type == typeid(ofVec3f)) {
    return createTypedTransition<ofVec3f>(param, endJsonVal);
  } else if (type == typeid(ofFloatColor)) {
    return createTypedTransition<ofFloatColor>(param, endJsonVal);
  } else {
    getLog().logWarning("Cannot transition param " + param.getKey() + " of type " + type.name());
    return nullptr;
  }
}

class CurrentToPresetLoader
: public NonCopyable {
public:
  using Storage = ParamTransitionSet::Storage;

  CurrentToPresetLoader(Storage& transitions)
  : _transitions(transitions) { }

  void load(Params& params,
            const ofJson& presetValues);

private:
  void add(TParamBase& param, const ofJson& endJsonVal) {
    auto transition = createTransition(param, endJsonVal);
    if (transition) {
      getLog().logNotice([&](ofLog& log) {
        log << "Added transition: " << *transition;
      });
      _transitions.push_back(transition);
    }
  }

  Storage& _transitions;
};

void CurrentToPresetLoader::load(Params &params,
                                 const ofJson &presetValues) {
  if (!presetValues.is_object()) {
    return;
  }
  for (auto param : params.getParamBases()) {
    auto iter = presetValues.find(param->getKey());
    if (iter == presetValues.end()) {
      continue;
    }
    const auto& presetVal = iter.value();
    if (presetVal.is_null()) {
      continue;
    }
    if (param->isGroup()) {
      auto subParams = dynamic_cast<Params*>(param);
      if (subParams != nullptr && presetVal.is_object()) {
        load(*subParams, presetVal);
      }
    }
    add(*param, presetVal);
  }
}

void ParamTransitionSet::loadCurrentToPreset(Params &params,
                                             const ParamPreset &preset) {
  CurrentToPresetLoader loader(_transitions);
  loader.load(params,
              preset.values());
}

void ParamTransitionSet::outputFields(std::ostream &os) const {
  if (!name().empty()) {
    os << "name: " << name() << ", ";
  }
  os << "transitions: [";
  for (const auto transition : _transitions) {
    os << *transition;
    os << ", ";
  }
  os << "]";
}
