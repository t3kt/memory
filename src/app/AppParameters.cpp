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
: Params("Debug") {
  add(showLog.set("Show Log", false));
  add(showBounds.set("Show Bounds", false));
  add(showStatus.set("Show Status", true));
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

MemoryAppParameters::MemoryAppParameters()
: bounds("Bounds") {
  add(clock);
  add(observers);
  add(occurrences);
  add(animations);
  add(colors);
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
