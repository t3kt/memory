//
//  Status.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Status_h
#define Status_h

#include <vector>
#include <utility>
#include <ofxTextAlignTTF.h>
#include "State.h"

class StatusInfoController {
public:
  StatusInfoController();

  void draw(const State& state);
private:
  ofxTextAlignTTF _text;
};

#endif /* Status_h */
