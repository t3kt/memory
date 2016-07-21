//
//  ParamsGui.h
//  memory
//
//  Created by tekt on 7/19/16.
//
//

#ifndef ParamsGui_h
#define ParamsGui_h

#include <memory>
#include <ofxControl.h>
#include <vector>
#include "Params.h"

class ParamGui {
public:
  ParamGui() {}
  virtual ~ParamGui() {}

  virtual void addToParent(ofxControlWidget* parent) = 0;
};

using ParamGuiList = std::vector<std::shared_ptr<ParamGui>>;

template<typename T>
class AbstractParamGuiControl
: public ParamGui {
public:
  AbstractParamGuiControl(TParam<T>& param)
  : _param(param)
  , _storage(param.get()) { }
protected:
  T _storage;
  TParam<T>& _param;
};

template<typename T>
class ParamGuiControl;

template<>
class ParamGuiControl<bool>
: public AbstractParamGuiControl<bool> {
public:
  using ParamT = TParam<bool>;

  ParamGuiControl(ParamT& param)
  : AbstractParamGuiControl<bool>(param) { }

  void addToParent(ofxControlWidget* parent) override {
    parent->addToggle(_param.getName(),
                      &_storage,
                      this,
                      &ParamGuiControl<bool>::onToggleEvent);
  }
private:
  void onToggleEvent(ofxControlButtonEventArgs& e) {
    _param.set(e.value);
  }
};

std::shared_ptr<ParamGui> createParamGuiControl(TParamBase& param);

class ParamsGui
: public ParamGui {
public:
  ParamsGui(Params& params) : _params(params) { }

  virtual void addToParent(ofxControlWidget* parent) override;
protected:
  virtual void build();
  void addControlsForParams();

  ofxControlWidget _root;
  Params& _params;
  ParamGuiList _controls;
};

template<typename ParamsT>
class TypedParamsGui
: public ParamGui {
public:
  TypedParamsGui(ParamsT& params)
  : _params(params) {}

  void addToParent(ofxControlWidget* parent) override {
    parent->addWidget(&_root);
    build();
  }

protected:
  virtual void build() = 0;

  ofxControlWidget _root;
  ParamsT& _params;
};

#endif /* ParamsGui_h */
