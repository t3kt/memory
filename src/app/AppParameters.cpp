//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

MemoryAppParameters::MemoryAppParameters() {
  paramGroup.add(observer.paramGroup);
  paramGroup.add(occurrence.paramGroup);
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(paramGroup);
  observer.initPanel(gui.getGroup(observer.paramGroup.getName()));
  occurrence.initPanel(gui.getGroup(occurrence.paramGroup.getName()));
}
