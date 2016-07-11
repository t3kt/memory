#include <memory>
#include <ofMain.h>
#include <ofAppGLFWWindow.h>
#include "AppParameters.h"
#include "ControlApp.h"
#include "SimulationApp.h"

const int CONTROLS_WIDTH = 300;

int main( ){
  ofGLFWWindowSettings simWinSettings;
  simWinSettings.width = 1100;
  simWinSettings.height = 800;
  simWinSettings.resizable = false;
  simWinSettings.setPosition(ofVec3f(CONTROLS_WIDTH + 5, 0));
  auto simWindow = ofCreateWindow(simWinSettings);

  ofGLFWWindowSettings ctrlWinSettings;
  ctrlWinSettings.width = CONTROLS_WIDTH;
  ctrlWinSettings.height = 800;
  ctrlWinSettings.resizable = true;
  ctrlWinSettings.setPosition(ofVec3f(0, 0));
  auto ctrlWindow = ofCreateWindow(ctrlWinSettings);

  MemoryAppParameters appParams;

  auto simApp = std::make_shared<SimulationApp>(appParams);

  auto ctrlApp = std::make_shared<ControlApp>(appParams, *simApp);

  ofRunApp(ctrlWindow, ctrlApp);
  ofRunApp(simWindow, simApp);
  ofRunMainLoop();
}
