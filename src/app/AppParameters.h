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

#include "ObserverEntity.h"
#include "OccurrenceEntity.h"

class MemoryAppParameters {
public:
  MemoryAppParameters();
  
  ofParameterGroup paramGroup;
  ObserverEntity::Params observer;
  OccurrenceEntity::Params occurrence;
};

#endif /* defined(__behavior__AppParameters__) */
