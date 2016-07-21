//
//  CoreParamsGui.h
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#ifndef CoreParamsGui_h
#define CoreParamsGui_h

#include <memory>
#include <ofxControl.h>
#include "AppParameters.h"
#include "ParamsGui.h"

class DebugParamsGui
: public ParamGui {
public:
  DebugParamsGui(DebugParams& params)
  : _params(params)
  , _root(params.getName()) { }

  void addToParent(ofxControlWidget* parent) override;
private:
  void onToggleEvent(ofxControlButtonEventArgs& event);
  ofxControlToggle* addToggle(TParam<bool>& param,
                              bool& storage);

  DebugParams& _params;

  struct {
    bool loggingEnabled;
    bool showBounds;
    bool showStatus;
    bool showPhysics;
//    bool inspect;
  } _storage;

  ofxControlWidget _root;
  ofxControlToggle* _loggingEnabledToggle;
  ofxControlToggle* _showBoundsToggle;
  ofxControlToggle* _showStatusToggle;
  ofxControlToggle* _showPhysicsToggle;
//  ofxControlToggle* _inspectToggle;
};

class DebugParamsGui_2
: public ParamGui {
public:
  DebugParamsGui_2(DebugParams& params)
  : _params(params)
  , _root(params.getName())
  , _loggingEnabled(params.loggingEnabled)
  , _showBounds(params.showBounds)
  , _showStatus(params.showStatus)
  , _showPhysics(params.showPhysics) { }

  void addToParent(ofxControlWidget* parent) override;
private:
  DebugParams& _params;
  ofxControlWidget _root;
  ParamGuiControl<bool> _loggingEnabled;
  ParamGuiControl<bool> _showBounds;
  ParamGuiControl<bool> _showStatus;
  ParamGuiControl<bool> _showPhysics;
};

class CoreParamsGui {
public:
  void setup();
  void addTo(ofxControlWidget& parent);
private:
  ofxControlWidget _root;
};

#endif /* CoreParamsGui_h */
