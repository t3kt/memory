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
#include "Params.h"
#include "Bounds.h"
#include "Clock.h"
#include "Colors.h"

class DebugParams : public Params {
public:
  DebugParams();

  Json to_json() const override;
  void read_json(const Json& obj) override;

  bool showLog() const { return _showLog.get(); }
  bool showBounds() const { return _showBounds.get(); }
  bool showStatus() const { return _showStatus.get(); }

  void setShowLog(bool showLog) { _showLog.set(showLog); }
  void setShowBounds(bool showBounds) { _showBounds.set(showBounds); }
  void setShowStatus(bool showStatus) { _showStatus.set(showStatus); }

private:
  TParam<bool> _showLog;
  TParam<bool> _showBounds;
  TParam<bool> _showStatus;
};

class CameraParams : public Params {
public:
  CameraParams();

  Json to_json() const override;
  void read_json(const Json& obj) override;

  void resetToDefaults() override {
    _spinEnabled.resetToDefault();
    _spinRate.resetToDefault();
  }

  bool hasDefaults() const override { return true; }

  bool spinEnabled() const { return _spinEnabled.get(); }
  const ofVec3f& spinRate() const { return _spinRate.get(); }

private:
  TParam<bool> _spinEnabled;
  TParam<ofVec3f> _spinRate;
};

class CoreParams : public Params {
public:
  CoreParams();

  void resetToDefaults() override {
    clock.resetToDefaults();
    camera.resetToDefaults();
    bounds.resetToDefaults();
    debug.resetToDefaults();
  }

  bool hasDefaults() const override { return true; }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  Clock::Params clock;
  CameraParams camera;
  SimpleCubeBounds bounds;
  DebugParams debug;

};

class MemoryAppParameters : public Params {
public:
  MemoryAppParameters();

  void readFromFile(std::string filepath);
  void writeToFile(std::string filepath) const;

  Json to_json() const override;
  void read_json(const Json& obj) override;

  void resetToDefaults() override {
    core.resetToDefaults();
    colors.resetToDefaults();
    animations.resetToDefaults();
    observers.resetToDefaults();
    occurrences.resetToDefaults();
  }

  bool hasDefaults() const override { return true; }

#ifdef ENABLE_SYPHON
  bool syphonEnabled() const { return _syphonEnabled.get(); }
#endif

  CoreParams core;
  ColorTheme colors;
  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
private:
#ifdef ENABLE_SYPHON
  TParam<bool> _syphonEnabled;
#endif

  friend class AppGuiImpl;
};

#endif /* defined(__behavior__AppParameters__) */
