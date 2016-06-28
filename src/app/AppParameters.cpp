//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"

DebugParams::DebugParams()
: Params("Debug") {
  add(showLog.set("Show Log?", false));
  add(drawBounds.set("Show Bounds?", false));
}

MemoryAppParameters::MemoryAppParameters()
: bounds("Bounds") {
  add(observers);
  add(occurrences);
  add(animations);
  add(bounds
      .set(6)
      .setParamRange(0, 10));
  add(debug);
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(*this);
  observers.initPanel(gui.getGroup(observers.getName()));
  occurrences.initPanel(gui.getGroup(occurrences.getName()));
  animations.initPanel(gui.getGroup(animations.getName()));
  debug.initPanel(gui.getGroup(debug.getName()));
}
