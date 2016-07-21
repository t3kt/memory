//
//  ControlController.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "AppParameters.h"
#include "Common.h"
#include "ControlController.h"
#include "ControlRouter.h"
#include "MidiDevice.h"

ControlController::ControlController(MemoryAppParameters& appParams)
: _appParams(appParams)
, _params(appParams.core.control) { }

void ControlController::setup() {
  _router = std::make_shared<ControlRouter>(_appParams);
  _twister = std::make_shared<MidiDevice>("twister",
                                          "Midi Fighter Twister",
                                          "Midi Fighter Twister",
                                          _params.midi.twister);
  _router->setup({_twister});
}

void ControlController::update() {
}
