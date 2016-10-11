//
//  AppSystem.h
//
//  Top-level application component. It owns the Context instance
//  and it provides centralized access to various global objects.
//

#ifndef AppSystem_h
#define AppSystem_h

#include <memory>
#include <ofAppGLFWWindow.h>
#include <ofEvents.h>
#include <ofSystemUtils.h>
#include "../app/AppActions.h"
#include "../app/AppParameters.h"
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/Events.h"
#include "../core/Logging.h"

class ActionsController;
class SimulationApp;

using FileAction = std::function<bool(ofFileDialogResult&)>;

class AppSystem
: public NonCopyable {
public:
  static void initialize();
  static AppSystem& get();

  AppSystem()
  : _log(_appParams.debug.logging) { }

  void main();

  MemoryAppParameters& params() { return _appParams; }

  std::shared_ptr<ofAppGLFWWindow>& simulationWindow() {
    return _simulationWindow;
  }

  SimulationApp* simulation() { return _simulationApp.get(); }

  Context& context() { return _context; }
  const Context& context() const { return _context; }

  LoggingController& log() { return _log; }

  ActionsController& actions();

  bool performAction(AppAction action);

  bool handleKeyPressed(ofKeyEventArgs& event);

  AppActionEvent appActionTriggered;

  bool performFileSaveAction(FileAction action,
                             std::string messageName,
                             std::string defaultName);
  bool performFileLoadAction(FileAction action,
                             std::string windowTitle = "",
                             std::string defaultPath = "");

private:
  void setup();

  MemoryAppParameters _appParams;
  Context _context;
  std::shared_ptr<ofAppGLFWWindow> _simulationWindow;
  std::shared_ptr<SimulationApp> _simulationApp;
  LoggingController _log;
};

#endif /* AppSystem_h */
