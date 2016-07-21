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
  , _storage(param.get()) {
    param.changed.addListener([this](T& e) {
      setControlValue(e);
    }, this);
  }

  ~AbstractParamGuiControl() override {
    _param.changed.removeListeners(this);
  }
protected:
  virtual void setControlValue(const T& value) = 0;
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
    _toggle =
    parent->addToggle(_param.getName(),
                      &_storage,
                      this,
                      &ParamGuiControl<bool>::onToggleEvent);
  }
private:
  void onToggleEvent(ofxControlButtonEventArgs& e) {
    _param.set(e.value);
  }
  void setControlValue(const bool& value) override {
    _toggle->setValue(value);
  }
  ofxControlToggle* _toggle;
};

std::shared_ptr<ParamGui> createParamGuiControl(TParamBase& param);

class AbstractParamsGui
: public ParamGui {
public:
  virtual void addToParent(ofxControlWidget* parent) override;
protected:
  virtual void build() = 0;
  void addControlsForParams(Params& params);

  ofxControlWidget _root;
  ParamGuiList _controls;
};

template<typename ParamsT>
class ParamsGui
: public AbstractParamsGui {
public:
  ParamsGui(ParamsT& params)
  : _params(params) {}

protected:
  virtual void build() override {
    _root.setName(_params.getName());
    addControlsForParams(_params);
  }

  ParamsT& _params;
};

#endif /* ParamsGui_h */
