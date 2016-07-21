//
//  AppParameters.h
//  memory
//
//  Created by tekt on 1/25/15.
//
//

#ifndef AppParameters_h
#define AppParameters_h

#include <ofParameterGroup.h>

#include "ObserversController.h"
#include "OccurrencesController.h"
#include "NavigatorsController.h"
#include "AnimationsController.h"
#include "RenderingController.h"
#include "PhysicsController.h"
#include "Params.h"
#include "Bounds.h"
#include "Clock.h"
#include "Colors.h"
#include "Events.h"
#include "ControlController.h"
#include "InspectionController.h"

class DebugParams : public Params {
public:
  DebugParams() {
    add(loggingEnabled
        .setKey("loggingEnabled")
        .setName("Logging Enabled")
        .setValueAndDefault(false));
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
  }

  TParam<bool> loggingEnabled;
  TParam<bool> showBounds;
  TParam<bool> showStatus;
  TParam<bool> showPhysics;
  InspectionController::Params inspect;
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
    add(bounds
        .setKey("bounds")
        .setName("Bounds"));
    add(debug
        .setKey("debug")
        .setName("Debug"));
    add(output
        .setKey("output")
        .setName("Output"));
    add(control
        .setKey("control")
        .setName("Control"));
  }

  Clock::Params clock;
  Bounds bounds;
  DebugParams debug;
  OutputParams output;
  ControlController::Params control;
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
  }

  CoreParams core;
  ColorTheme colors;
  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
  NavigatorsController::Params navigators;
  RenderingController::Params rendering;
  PhysicsController::Params physics;
};

#endif /* AppParameters_h */
