//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

MemoryAppParameters::MemoryAppParameters() {
  add(observers);
  add(occurrences);
  add(animations);
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(*this);
  observers.initPanel(gui.getGroup(observers.getName()));
  occurrences.initPanel(gui.getGroup(occurrences.getName()));
  animations.initPanel(gui.getGroup(animations.getName()));
}
