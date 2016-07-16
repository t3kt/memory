//
//  MidiController.hpp
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
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(_twisterEnabled
          .setKey("twisterEnabled")
          .setName("MF Twister Enabled")
          .setValueAndDefault(false));
      setEnabledValueAndDefault(false);
      _twisterEnabled.addListener(this,
                                  &Params::onTwisterEnabledChanged);
    }

    ValueEvent<bool> twisterEnabledChanged;

    bool twisterEnabled() const { return _twisterEnabled.get(); }

  private:
    void onTwisterEnabledChanged(bool& value) {
      ValueEventArgs<bool> e(value);
      twisterEnabledChanged.notifyListenersUntilHandled(e);
    }

    TParam<bool> _twisterEnabled;
  };

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
