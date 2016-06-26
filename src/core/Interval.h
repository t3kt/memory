//
//  Interval.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef Interval_h
#define Interval_h

#include <ofParameter.h>
#include <string>
#include "Params.h"
#include "ValueSupplier.h"
#include "State.h"

class Interval {
public:
  class Params : public ::Params {
  public:
    Params(std::string name);
    
    RandomValueSupplier<float> interval;
  };
  
  Interval(const Params& params, const State& state);
  
  bool check(const State& state);
  
private:
  void loadNext(const State& state);
  
  const Params& _params;
  float _nextTime;
};

#endif /* Interval_h */
