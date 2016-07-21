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

using DebugParamsGui = ParamsGui<DebugParams>;
using ClockParamsGui = ParamsGui<Clock::Params>;
using BoundsGui = ParamsGui<Bounds>;


class CoreParamsGui
: public ParamsGui<CoreParams> {
public:
  CoreParamsGui(CoreParams& params)
  : ParamsGui(params)
  , _clock(params.clock)
  , _bounds(params.bounds)
  , _debug(params.debug) { }
protected:
  virtual void build() override {
    _clock.addToParent(&_root);
    _bounds.addToParent(&_root);
    _debug.addToParent(&_root);
  }

  ClockParamsGui _clock;
  BoundsGui _bounds;
  DebugParamsGui _debug;
};

#endif /* CoreParamsGui_h */
