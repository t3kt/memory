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
  DebugParams();

  bool showLog() const { return _showLog.get(); }
  bool showBounds() const { return _showBounds.get(); }
  bool showStatus() const { return _showStatus.get(); }
  bool showPhysics() const { return _showPhysics.get(); }

  void setShowLog(bool showLog) { _showLog.set(showLog); }
  void setShowBounds(bool showBounds) { _showBounds.set(showBounds); }
  void setShowStatus(bool showStatus) { _showStatus.set(showStatus); }
  void setShowPhysics(bool showPhysics) { _showPhysics.set(showPhysics); }

private:
  TParam<bool> _showLog;
  TParam<bool> _showBounds;
  TParam<bool> _showStatus;
  TParam<bool> _showPhysics;
};

class CoreParams : public Params {
public:
  CoreParams();

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
  MemoryAppParameters();

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
