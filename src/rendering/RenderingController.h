//
//  RenderingController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef RenderingController_h
#define RenderingController_h

#include "Params.h"
#include "AppParameters.h"
#include "State.h"
#include "Colors.h"

class RenderingController {
public:
  static shared_ptr<RenderingController> create(const CameraParams& cameraParams, const ColorTheme& colors);

  virtual void update(const State& state) = 0;
  virtual void beginDraw(const State& state) = 0;
  virtual void endDraw(const State& state) = 0;
  virtual void resetCamera() = 0;
};

#endif /* RenderingController_h */
