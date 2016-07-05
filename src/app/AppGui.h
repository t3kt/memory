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
#include <ofxLiquidEvent.h>

#include "AppParameters.h"

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

  std::shared_ptr<ofxBaseGui> _actions;
  std::shared_ptr<ofxBaseGui> _settings;
};

#endif /* AppGui_h */
