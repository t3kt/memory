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
#include "Params.h"

class MemoryAppParameters;
class ofxMidiFighterTwister;
class MidiRouter;

class MidiController {
public:
  using Params = ParamsWithEnabled;

  MidiController(MemoryAppParameters& appParams);

  void setup();
  void update();

private:
  void initializeTwister();
  void destroyTwister();
  void initOrDestroyTwister();

  MemoryAppParameters& _appParams;
  Params& _params;
  std::shared_ptr<MidiRouter> _router;
  std::shared_ptr<ofxMidiFighterTwister> _twister;
};

#endif /* MidiController_h */
