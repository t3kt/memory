//
//  AppParameters.h
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#ifndef __behavior__AppParameters__
#define __behavior__AppParameters__

#include <ofParameterGroup.h>
#include <ofxPanel.h>
#include <ofxDatGui.h>

#include "ObserversController.h"
#include "OccurrencesController.h"
#include "AnimationsController.h"
#include "Params.h"
#include "Bounds.h"
#include "Clock.h"
#include "Colors.h"

class DebugParams : public Params {
public:
  DebugParams();

  ofParameter<bool> showLog;
  ofParameter<bool> showBounds;
  ofParameter<bool> showStatus;

protected:
  ParamsGui* createGui() override;
};

class MemoryAppParameters : public Params {
public:
  MemoryAppParameters();
  
  void initGui(ofxPanel& gui);

  Clock::Params clock;
  DebugParams debug;
  ColorTheme colors;
  SimpleCubeBounds bounds;
  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
};

#endif /* defined(__behavior__AppParameters__) */
