//
//  AppGui.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef AppGui_h
#define AppGui_h

#include <ofxDatGui.h>
#include "AppParameters.h"

class AppGui {
public:
  AppGui(MemoryAppParameters& appParams);

  void setup();
  void update();
  void draw();

private:
  void add(Params& params);

  MemoryAppParameters& _appParams;
  ofxDatGui* _gui;
};

#endif /* AppGui_h */
