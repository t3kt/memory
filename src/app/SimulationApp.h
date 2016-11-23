//
//  SimulationApp.h
//

#pragma once

#include <ofMain.h>
#include <ofxTCommon.h>
#include "../core/Actions.h"
#include "../core/AnimationsController.h"
#include "../app/AppGui.h"
#include "../app/AppParameters.h"
#include "../core/Clock.h"
#include "../core/Component.h"
#include "../core/Context.h"
#include "../core/InspectionController.h"
#include "../control/MidiController.h"
#include "../navigation/NavigatorsController.h"
#include "../core/NodesController.h"
#include "../core/ObserversController.h"
#include "../core/OccurrencesController.h"
#include "../control/OscController.h"
#include "../control/ParametersController.h"
#include "../physics/PhysicsController.h"
#include "../rendering/RenderingController.h"
#include "../core/SimulationEvents.h"
#include "../core/State.h"
#include "../core/Status.h"
#include "../core/TimingController.h"

class BoundsController;
class EventLoggers;
class ScenesController;
class CommandsController;

class SimulationApp
: public ofBaseApp
, public ofxTCommon::NonCopyable {
public:
  SimulationApp(MemoryAppParameters& appParams,
                Context& context,
                std::shared_ptr<ofAppGLFWWindow> window)
  : _appParams(appParams)
  , _context(context)
  , _window(window) { }

  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(ofKeyEventArgs& eventl) override;

  SimulationEvents& getEvents() { return _events; }

  ActionsController& actions() { return *_actions; }

  const BoundsController& bounds() const {
    return _physics->bounds();
  }

  ParametersController& parameters() { return *_paramsController; }

  TimingController& timing() { return *_timing; }

  CommandsController& commands() { return *_commands; }

  AppGui& gui() { return *_gui; }

private:
  void dumpEntityState();
  void loadEntityState();
  void saveEntityState();

  void updateLogState();

  Context& _context;
  MemoryAppParameters& _appParams;
  SimulationEvents _events;
  std::shared_ptr<EventLoggers> _eventLoggers;
  std::shared_ptr<ofAppGLFWWindow> _window;
  std::shared_ptr<AppGui> _gui;
  std::shared_ptr<ActionsController> _actions;
  std::shared_ptr<MidiController> _midi;
  std::shared_ptr<OscController> _osc;
  std::shared_ptr<TimingController> _timing;
  std::shared_ptr<PhysicsController> _physics;
  std::shared_ptr<NodesController> _nodes;
  std::shared_ptr<ObserversController> _observers;
  std::shared_ptr<OccurrencesController> _occurrences;
  std::shared_ptr<AnimationsController> _animations;
  std::shared_ptr<NavigatorsController> _navigators;
  std::shared_ptr<RenderingController> _renderingController;
  std::shared_ptr<InspectionController> _inspectionController;
  std::shared_ptr<StatusInfoController> _statusController;
  std::shared_ptr<ParametersController> _paramsController;
  std::shared_ptr<CommandsController> _commands;
  std::shared_ptr<ScenesController> _scenes;
  ComponentCollection<ComponentBase> _components;
};

