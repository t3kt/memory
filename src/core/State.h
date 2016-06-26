//
//  State.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__State__
#define __behavior__State__

#include "Common.h"

class State : public Outputable {
public:
  State();
  void updateTime();
  
  void output(std::ostream& os) const override;
  
  float time;
  float timeDelta;
};

#endif /* defined(__behavior__State__) */
