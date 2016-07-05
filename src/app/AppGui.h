//
//  AppGui.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef AppGui_h
#define AppGui_h

#include <memory>
#include <vector>
#include <ofxLiquidEvent.h>

#include "AppParameters.h"

class ofxFpsPlotter;

class AppGuiImpl;

class AppGui {
public:
  AppGui(MemoryAppParameters& appParams);

  void setup();
  void update();
  void draw();

  ofxLiquidEvent<void> onLoad;
  ofxLiquidEvent<void> onSave;

private:
  MemoryAppParameters& _appParams;
  std::shared_ptr<AppGuiImpl> _impl;
};

#endif /* AppGui_h */
