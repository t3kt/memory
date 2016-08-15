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
#include "../core/Events.h"
#include "../control/MidiCommon.h"
#include "../core/Params.h"

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
  std::shared_ptr<MidiDevice> _max;
};

#endif /* MidiController_h */
