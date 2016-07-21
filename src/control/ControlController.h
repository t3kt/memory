//
//  ControlController.h
//  memory
//
//  Created by tekt on 7/15/16.
//
//

#ifndef ControlController_h
#define ControlController_h

#include <memory>
#include "ControlCommon.h"
#include "Events.h"
#include "Params.h"

class MemoryAppParameters;
class MidiDevice;
class ControlRouter;

class ControlController {
public:
  using Params = ControlParams;

  ControlController(MemoryAppParameters& appParams);

  void setup();
  void update();

private:
  MemoryAppParameters& _appParams;
  Params& _params;
  std::shared_ptr<ControlRouter> _router;
  std::shared_ptr<MidiDevice> _twister;
};

#endif /* ControlController_h */
