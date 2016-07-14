//
//  ControlApp.h
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#ifndef ControlApp_h
#define ControlApp_h

#include <memory>
#include <ofMain.h>
#include "AppActions.h"
#include "AppGui.h"
#include "AppParameters.h"
#include "Common.h"
#include "State.h"

class SimulationApp;
class EventLoggers;

class ControlApp
: public ofBaseApp
, public AppActionHandler {
public:
  ControlApp(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void attachSimulation(std::shared_ptr<SimulationApp> simulation);

  void setup() override;
  void draw() override;

  void keyPressed(int key) override;

  void performAction(AppAction action) override;
private:
  void updateLogState();

  void loadSettings();
  void saveSettings();

  MemoryAppParameters& _appParams;
  std::shared_ptr<SimulationApp> _simulation;
  std::shared_ptr<EventLoggers> _eventLoggers;
  std::shared_ptr<AppGui> _gui;
};

#endif /* ControlApp_h */
