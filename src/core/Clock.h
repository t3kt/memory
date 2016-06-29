//
//  Clock.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Clock_h
#define Clock_h

#include <ofParameter.h>
#include "State.h"
#include "Params.h"

class Clock {
public:
  class Params : public ::Params {
    Params();

    ofParameter<bool> paused;
    ofParameter<float> rate;
  };

  Clock(const Params& params, State& state);

  void setup();
  void update();
private:
  State& _state;
  const Params& _params;
};

#endif /* Clock_h */
