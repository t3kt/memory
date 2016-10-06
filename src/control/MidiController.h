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
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/Events.h"
#include "../control/MidiCommon.h"
#include "../core/Params.h"

class MemoryAppParameters;
class MidiDevice;
class MidiRouter;

class MidiController
: public NonCopyable
, public ComponentBase {
public:
  using Params = MidiParams;

  MidiController(MemoryAppParameters& appParams);

  void setup() override;
  void update() override;

private:
  MemoryAppParameters& _appParams;
  Params& _params;
  std::shared_ptr<MidiRouter> _router;
  std::shared_ptr<MidiDevice> _twister;
  std::shared_ptr<MidiDevice> _max;
};

#endif /* MidiController_h */
