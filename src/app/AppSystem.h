//
//  AppSystem.h
//  memory
//
//  Created by tekt on 7/14/16.
//
//

#ifndef AppSystem_h
#define AppSystem_h

#include <memory>
#include <ofAppGLFWWindow.h>
#include "AppParameters.h"

class SimulationApp;
class ControlApp;

class AppSystem {
public:
  static AppSystem& get();

  void main();
private:
  void setup();

  MemoryAppParameters _appParams;
  std::shared_ptr<ofAppGLFWWindow> _simulationWindow;
  std::shared_ptr<ofAppGLFWWindow> _controlWindow;
  std::shared_ptr<SimulationApp> _simulationApp;
  std::shared_ptr<ControlApp> _controlApp;
};

#endif /* AppSystem_h */
