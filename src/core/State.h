//
//  State.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__State__
#define __behavior__State__

#include "../core/Common.h"

class ClockState
: public NonCopyable
, public Outputable {
public:
  ClockState()
  : localTime(0)
  , timeDelta(0)
  , running(true) { }

  float localTime;
  float timeDelta;
  bool running;
  float rate;

  std::string typeName() const override { return "ClockState"; }

protected:
  void outputFields(std::ostream& os) const override;
};

#endif /* defined(__behavior__State__) */
