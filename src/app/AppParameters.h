//
//  AppParameters.h
//  behavior
//
//  Created by tekt on 1/25/15.
//
//

#ifndef __behavior__AppParameters__
#define __behavior__AppParameters__

#include <ofParameterGroup.h>
#include <ofxGui.h>

#include "ObserversController.h"
#include "OccurrencesController.h"
#include "AnimationsController.h"
#include "RenderingController.h"
#include "PhysicsController.h"
#include "Params.h"
#include "Bounds.h"
#include "Clock.h"
#include "Colors.h"

class DebugParams : public Params {
public:
  DebugParams() {
    add(_loggingEnabled
        .setKey("loggingEnabled")
        .setName("Logging Enabled")
        .setValueAndDefault(false));
    add(_showBounds
        .setKey("showBounds")
        .setName("Show Bounds")
        .setValueAndDefault(false));
    add(_showStatus
        .setKey("showStatus")
        .setName("Show Status")
        .setValueAndDefault(true));
    add(_showPhysics
        .setKey("showPhysics")
        .setName("Show Physics")
        .setValueAndDefault(false));
  }

  bool loggingEnabled() const { return _loggingEnabled.get(); }
  bool showBounds() const { return _showBounds.get(); }
  bool showStatus() const { return _showStatus.get(); }
  bool showPhysics() const { return _showPhysics.get(); }

  void setLoggingEnabled(bool loggingEnabled) { _loggingEnabled.set(loggingEnabled); }
  void setShowBounds(bool showBounds) { _showBounds.set(showBounds); }
  void setShowStatus(bool showStatus) { _showStatus.set(showStatus); }
  void setShowPhysics(bool showPhysics) { _showPhysics.set(showPhysics); }

private:
  TParam<bool> _loggingEnabled;
  TParam<bool> _showBounds;
  TParam<bool> _showStatus;
  TParam<bool> _showPhysics;
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
#ifdef ENABLE_SYPHON
    add(_syphonEnabled
        .setKey("syphonEnabled")
        .setName("Enable Syphon")
        .setValueAndDefault(false));
#endif
    bounds.setParamRange(0.5, 40);
  }

#ifdef ENABLE_SYPHON
  bool syphonEnabled() const { return _syphonEnabled.get(); }
#endif

  Clock::Params clock;
  SimpleCubeBounds bounds;
  DebugParams debug;

private:
#ifdef ENABLE_SYPHON
  TParam<bool> _syphonEnabled;
#endif

  friend class AppGuiImpl;
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

  void readFromFile(std::string filepath);
  void writeToFile(std::string filepath) const;

  CoreParams core;
  ColorTheme colors;
  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
  RenderingController::Params rendering;
  PhysicsController::Params physics;
};

#endif /* defined(__behavior__AppParameters__) */
