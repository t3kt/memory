//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

MemoryAppParameters::MemoryAppParameters() {
  paramGroup.add(observer);
  paramGroup.add(occurrence);
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(paramGroup);
  observer.initPanel(gui.getGroup(observer.getName()));
  occurrence.initPanel(gui.getGroup(occurrence.getName()));
}
