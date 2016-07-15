//
//  AppSystem.cpp
//  memory
//
//  Created by tekt on 7/14/16.
//
//

#include "AppSystem.h"
#include "ControlApp.h"
#include "SimulationApp.h"

const int CONTROLS_WIDTH = 250;

static AppSystem instance;

AppSystem& AppSystem::get() {
  return instance;
}

void AppSystem::setup() {
  ofGLFWWindowSettings simWinSettings;
  simWinSettings.width = 1100;
  simWinSettings.height = 800;
  simWinSettings.resizable = false;
  simWinSettings.setPosition(ofVec3f(CONTROLS_WIDTH + 5, 0));
  _simulationWindow =
  std::static_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(simWinSettings));

  ofGLFWWindowSettings ctrlWinSettings;
  ctrlWinSettings.width = CONTROLS_WIDTH;
  ctrlWinSettings.height = 800;
  ctrlWinSettings.resizable = true;
  ctrlWinSettings.setPosition(ofVec3f(0, 0));
  _controlWindow =
  std::static_pointer_cast<ofAppGLFWWindow>(ofCreateWindow(ctrlWinSettings));

  _simulationApp = std::make_shared<SimulationApp>(_appParams,
                                                   _simulationWindow);

  _controlApp = std::make_shared<ControlApp>(_appParams);

  _simulationApp->attachControls(_controlApp);
  _controlApp->attachSimulation(_simulationApp);
}

void AppSystem::main() {
  setup();

  ofRunApp(_controlWindow, _controlApp);
  ofRunApp(_simulationWindow, _simulationApp);
  ofRunMainLoop();
}
