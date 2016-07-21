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

#include "AppParameters.h"
#include "AppActions.h"

class AppGuiImpl;

class AppGui {
public:
  AppGui(MemoryAppParameters& appParams,
         AppActionHandler& actionHandler);

  void draw();
private:
  // intentionally leak this on destruction as a hack to get around
  // problems in the ofxControl desctructors
  AppGuiImpl* _impl;
};

#endif /* AppGui_h */
