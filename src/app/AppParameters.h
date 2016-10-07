//
//  AppParameters.h
//
//  Parameters which control the behavior of the application.
//

#ifndef AppParameters_h
#define AppParameters_h

#include "../core/AnimationsController.h"
#include "../core/Clock.h"
#include "../rendering/Colors.h"
#include "../core/Events.h"
#include "../core/InspectionController.h"
#include "../core/Logging.h"
#include "../control/MidiController.h"
#include "../navigation/NavigatorsController.h"
#include "../core/ObserversController.h"
#include "../core/OccurrencesController.h"
#include "../control/OscController.h"
#include "../core/Params.h"
#include "../physics/PhysicsController.h"
#include "../rendering/RenderingController.h"

class DebugParams : public Params {
public:
  DebugParams() {
    add(showBounds
        .setKey("showBounds")
        .setName("Show Bounds")
        .setValueAndDefault(false));
    add(showStatus
        .setKey("showStatus")
        .setName("Show Status")
        .setValueAndDefault(true));
    add(showPhysics
        .setKey("showPhysics")
        .setName("Show Physics")
        .setValueAndDefault(false));
    add(inspect
        .setKey("inspect")
        .setName("Inspect"));
    add(logging
        .setKey("logging")
        .setName("Logging"));
  }

  TParam<bool> showBounds;
  TParam<bool> showStatus;
  TParam<bool> showPhysics;
  InspectionController::Params inspect;
  LoggingController::Params logging;
};

class OutputParams : public Params {
public:
  OutputParams() {
    add(fullscreen
        .setKey("fullscreen")
        .setName("Fullscreen"));
    add(externalEnabled
        .setKey("externalEnabled")
        .setName("Enable External Send")
        .setValueAndDefault(false));
  }

  TParam<bool> fullscreen;
  TParam<bool> externalEnabled;
};

class CoreParams : public Params {
public:
  CoreParams() {
    add(clock
        .setKey("clock")
        .setName("Clock"));
    add(output
        .setKey("output")
        .setName("Output"));
    add(midi
        .setKey("midi")
        .setName("Midi")
        .setSupportsOsc(false));
    add(osc
        .setKey("osc")
        .setName("OSC")
        .setSupportsOsc(false));
  }

  Clock::Params clock;
  OutputParams output;
  MidiController::Params midi;
  OscController::Params osc;
};

class MemoryAppParameters : public Params {
public:
  MemoryAppParameters() {
    add(core
        .setKey("core")
        .setName("Core"));
    add(observers
        .setKey("observers")
        .setName("Observers"));
    add(occurrences
        .setKey("occurrences")
        .setName("Occurrences"));
    add(navigators
        .setKey("navigators")
        .setName("Navigators"));
    add(animations
        .setKey("animations")
        .setName("Animations"));
    add(colors
        .setKey("colors")
        .setName("Colors"));
    add(rendering
        .setKey("rendering")
        .setName("Rendering"));
    add(physics
        .setKey("physics")
        .setName("Physics"));
    add(debug
        .setKey("debug")
        .setName("Debug"));
  }

  CoreParams core;
  ColorTheme colors;
  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
  NavigatorsController::Params navigators;
  RenderingController::Params rendering;
  PhysicsController::Params physics;
  DebugParams debug;
};

#endif /* AppParameters_h */
