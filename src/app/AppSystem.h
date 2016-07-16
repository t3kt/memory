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
#include <ofEvents.h>
#include "AppActions.h"
#include "AppParameters.h"
#include "Events.h"

class SimulationApp;
class ControlApp;

class AppSystem {
public:
  static void initialize();
  static AppSystem& get();

  void main();

  MemoryAppParameters* params() { return &_appParams; }

  std::shared_ptr<ofAppGLFWWindow>& simulationWindow() {
    return _simulationWindow;
  }

  std::shared_ptr<ofAppGLFWWindow>& controlWindow() {
    return _controlWindow;
  }

  SimulationApp* simulation() { return _simulationApp.get(); }

  ControlApp* control() { return _controlApp.get(); }

  bool performAction(AppAction action);

  bool handleKeyPressed(ofKeyEventArgs& event);

  AppActionEvent appActionTriggered;

private:
  void setup();

  MemoryAppParameters _appParams;
  std::shared_ptr<ofAppGLFWWindow> _simulationWindow;
  std::shared_ptr<ofAppGLFWWindow> _controlWindow;
  std::shared_ptr<SimulationApp> _simulationApp;
  std::shared_ptr<ControlApp> _controlApp;
};

#endif /* AppSystem_h */
