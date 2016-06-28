//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

MemoryAppParameters::MemoryAppParameters()
: bounds("Bounds") {
  add(showLog.set("Show Log?", false));
  add(observers);
  add(occurrences);
  add(animations);
  add(bounds
      .set(2)
      .setParamRange(0, 10));
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(*this);
  observers.initPanel(gui.getGroup(observers.getName()));
  occurrences.initPanel(gui.getGroup(occurrences.getName()));
  animations.initPanel(gui.getGroup(animations.getName()));
}
