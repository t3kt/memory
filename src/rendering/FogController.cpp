//
//  FogController.cpp
//  memory
//
//  Created by tekt on 10/8/16.
//
//

#include <ofMain.h>
#include "../app/AppSystem.h"
#include "../rendering/FogController.h"

FogController::FogController(Params& params)
: _params(params)
, _colors(AppSystem::get().params().colors) { }

void FogController::beginDraw() {
  if (!_params.enabled.get()) {
    return;
  }
  GLfloat fogCol[4];
  if (_params.useBackgroundColor.get()) {
    const auto& color = _colors.background.get();
    fogCol[0] = color.r;
    fogCol[1] = color.g;
    fogCol[2] = color.b;
  } else {
    const auto& color = _colors.fog.get();
    fogCol[0] = color.r;
    fogCol[1] = color.g;
    fogCol[2] = color.b;
  }
  fogCol[3] = 1; // not used by opengl?

  glFogi(GL_FOG_MODE, GL_EXP2);
  glFogfv(GL_FOG_COLOR, fogCol);
  glFogf(GL_FOG_DENSITY, _params.density.get());
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glFogf(GL_FOG_START, _params.distance.lowValue.get());
  glFogf(GL_FOG_END, _params.distance.highValue.get());
  glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
  glEnable(GL_FOG);
}

void FogController::endDraw() {
  if (!_params.enabled.get()) {
    return;
  }
  glDisable(GL_FOG);
}
