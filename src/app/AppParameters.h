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

#include "ObserversController.h"
#include "OccurrencesController.h"
#include "AnimationsController.h"
#include "Params.h"

class MemoryAppParameters : public Params {
public:
  MemoryAppParameters();
  
  void initGui(ofxPanel& gui);

  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
};

#endif /* defined(__behavior__AppParameters__) */
