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

class DebugParamsGui {
public:
  DebugParamsGui(DebugParams& params)
  : _params(params)
  , _root(params.getName()) { }

  void setup(ofxControlWidget& parent);
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

class CoreParamsGui {
public:
  void setup();
  void addTo(ofxControlWidget& parent);
private:
  ofxControlWidget _root;
};

#endif /* CoreParamsGui_h */
