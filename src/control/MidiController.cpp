//
//  MidiController.cpp
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#include "AppParameters.h"
#include "Common.h"
#include "MidiController.h"
#include "MidiDevice.h"
#include "MidiRouter.h"

MidiController::MidiController(MemoryAppParameters& appParams)
: _appParams(appParams)
, _params(appParams.core.midi) { }

void MidiController::setup() {
  _router = std::make_shared<MidiRouter>(_appParams);
  _twister = std::make_shared<MidiDevice>("twister",
                                          "Midi Fighter Twister",
                                          "Midi Fighter Twister",
                                          _params.twister);
  _max = std::make_shared<MidiDevice>("max",
                                      "IAC Driver Virtual 2", // input from external
                                      "IAC Driver Virtual 1", // output to external
                                      _params.max);
  _router->setup({_twister, _max});
}

void MidiController::update() {
}
