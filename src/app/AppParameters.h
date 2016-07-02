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
#include <ofxPanel.h>
#include <ofxDatGui.h>

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

  TParam<bool> showLog;
  TParam<bool> showBounds;
  TParam<bool> showStatus;

protected:
  ParamsGui* createGui() override;
};

class CameraParams : public Params {
public:
  CameraParams();

  Json to_json() const override;
  void read_json(const Json& obj) override;

  TParam<bool> spinEnabled;
  TParam<ofVec3f> spinRate;
};

class MemoryAppParameters : public Params {
public:
  MemoryAppParameters();

  void readFromFile(std::string filepath);
  void writeToFile(std::string filepath) const;

  Json to_json() const override;
  void read_json(const Json& obj) override;

  void initGui(ofxPanel& gui);

  Clock::Params clock;
  DebugParams debug;
  ColorTheme colors;
  SimpleCubeBounds bounds;
  AnimationsController::Params animations;
  ObserversController::Params observers;
  OccurrencesController::Params occurrences;
  CameraParams camera;
};

#endif /* defined(__behavior__AppParameters__) */
