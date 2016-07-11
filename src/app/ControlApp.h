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
#include "SimulationApp.h"
#include "State.h"

class ControlApp : public ofBaseApp {
public:
  ControlApp(MemoryAppParameters& appParams,
             SimulationApp& simulation)
  : _appParams(appParams)
  , _simulation(simulation) { }

  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(int key) override;

  void performAction(AppAction action);
private:
  void loadSettings();
  void saveSettings();

  MemoryAppParameters& _appParams;
  SimulationApp& _simulation;
  std::shared_ptr<AppGui> _gui;
};

#endif /* ControlApp_h */
