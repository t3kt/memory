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

class State
: public Outputable
, public NonCopyable {
public:
  State()
  : running(true) { }
  
  float time;
  float timeDelta;
  int observerCount;
  int occurrenceCount;
  int animationCount;
  bool running;
  std::string typeName() const override { return "State"; }
protected:
  void outputFields(std::ostream& os) const override;
};

#endif /* defined(__behavior__State__) */
