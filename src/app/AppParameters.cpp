//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

MemoryAppParameters::MemoryAppParameters() {
  paramGroup.add(observers);
  paramGroup.add(occurrences);
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(paramGroup);
  observers.initPanel(gui.getGroup(observers.getName()));
  occurrences.initPanel(gui.getGroup(occurrences.getName()));
}
