//
//  AppParameters.cpp
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#include "AppParameters.h"
#include "ParamsGui.h"

DebugParams::DebugParams()
: Params() {
  add(showLog
      .setKey("showLog")
      .setName("Show Log")
      .setValueAndDefault(false));
  add(showBounds
      .setKey("showBounds")
      .setName("Show Bounds")
      .setValueAndDefault(false));
  add(showStatus
      .setKey("showStatus")
      .setName("Show Status")
      .setValueAndDefault(true));
}

class DebugParamsGui
: public AbstractParamsGui {
public:
  DebugParamsGui(DebugParams& params)
  : AbstractParamsGui(params)
  , _params(params) { }
protected:
  void addControls() override;
private:
  DebugParams& _params;
};

void DebugParamsGui::addControls() {
  addToggle(_params.showLog);
  addToggle(_params.showBounds);
  addToggle(_params.showStatus);
}

ParamsGui* DebugParams::createGui() {
  return new DebugParamsGui(*this);
}

CameraParams::CameraParams()
: ::Params() {
  add(spinEnabled
      .setKey("spinEnabled")
      .setName("Spin Enabled")
      .setValueAndDefault(true));
  add(spinRate
      .setKey("spinRate")
      .setName("Spin Rate")
      .setValueAndDefault(ofVec3f(2, 4, 5))
      .setRange(ofVec3f(-10), ofVec3f(10)));
}

MemoryAppParameters::MemoryAppParameters()
: ::Params() {
  add(clock
      .setKey("clock")
      .setName("Clock"));
  add(observers
      .setKey("observers")
      .setName("Observers"));
  add(occurrences
      .setKey("occurrences")
      .setName("Occurrences"));
  add(animations
      .setKey("animations")
      .setName("Animations"));
  add(colors
      .setKey("colors")
      .setName("Colors"));
  add(bounds
      .setKey("bounds")
      .setName("Bounds"));
  add(debug
      .setKey("debug")
      .setName("Debug"));
  add(camera
      .setKey("camera")
      .setName("Camera"));
}

void MemoryAppParameters::initGui(ofxPanel &gui) {
  gui.setup(*this);
  observers.initPanel(gui.getGroup(observers.getName()));
  occurrences.initPanel(gui.getGroup(occurrences.getName()));
  animations.initPanel(gui.getGroup(animations.getName()));
  debug.initPanel(gui.getGroup(debug.getName()));
}
