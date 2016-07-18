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
#include "Context.h"
#include "Events.h"
#include "State.h"

class SimulationApp;
class ControlApp;

class AppSystem {
public:
  static void initialize();
  static AppSystem& get();

  AppSystem()
  : _context(_appParams) {}

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

  State& state() { return _context.state; }
  const State& state() const { return _context.state; }

  Context& context() { return _context; }
  const Context& context() const { return _context; }

  bool performAction(AppAction action);

  bool handleKeyPressed(ofKeyEventArgs& event);

  AppActionEvent appActionTriggered;

private:
  void setup();

  MemoryAppParameters _appParams;
  Context _context;
  std::shared_ptr<ofAppGLFWWindow> _simulationWindow;
  std::shared_ptr<ofAppGLFWWindow> _controlWindow;
  std::shared_ptr<SimulationApp> _simulationApp;
  std::shared_ptr<ControlApp> _controlApp;
};

#endif /* AppSystem_h */
