//
//  MidiController.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef MidiController_h
#define MidiController_h

#include <memory>
#include "Events.h"
#include "MidiCommon.h"
#include "Params.h"

class MemoryAppParameters;
class MidiDevice;
class MidiRouter;

class MidiController {
public:
  using Params = MidiParams;

  MidiController(MemoryAppParameters& appParams);

  void setup();
  void update();

private:
  MemoryAppParameters& _appParams;
  Params& _params;
  std::shared_ptr<MidiRouter> _router;
  std::shared_ptr<MidiDevice> _twister;
};

#endif /* MidiController_h */
