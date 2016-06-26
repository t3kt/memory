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
  paramGroup.add(occurrence);
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(paramGroup);
  observers.initPanel(gui.getGroup(observers.getName()));
  occurrence.initPanel(gui.getGroup(occurrence.getName()));
}
