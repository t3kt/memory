//
//  SimulationApp.h
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#ifndef SimulationApp_h
#define SimulationApp_h

#include <ofMain.h>
#include "AnimationsController.h"
#include "AppActions.h"
#include "AppParameters.h"
#include "Clock.h"
#include "Common.h"
#include "ObserversController.h"
#include "OccurrencesController.h"
#include "PhysicsController.h"
#include "RenderingController.h"
#include "State.h"
#include "Status.h"

#ifdef ENABLE_SYPHON
#include <ofxSyphon.h>
#endif

class ControlApp;

class SimulationApp : public ofBaseApp {
public:
  SimulationApp(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void attachControls(std::shared_ptr<ControlApp> control);

  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(int key) override;

  void performAction(AppAction action);
private:
  State _state;
  MemoryAppParameters& _appParams;
  std::shared_ptr<ControlApp> _control;
  std::shared_ptr<ObserversController> _observers;
  std::shared_ptr<OccurrencesController> _occurrences;
  std::shared_ptr<AnimationsController> _animations;
  std::shared_ptr<StatusInfoController> _statusController;
  std::shared_ptr<RenderingController> _renderingController;
  std::shared_ptr<PhysicsController> _physics;
  std::shared_ptr<Clock> _clock;
#ifdef ENABLE_SYPHON
  ofxSyphonServer _syphonServer;
#endif
};

#endif /* SimulationApp_h */
