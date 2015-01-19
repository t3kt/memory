//
//  BehaviorApp.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__BehaviorApp__
#define __behavior__BehaviorApp__

#include <ofMain.h>
#include "State.h"

class BehaviorApp : public ofBaseApp {
public:
  void setup() override;
  void update() override;
  void draw() override;
private:
  State _state;
  ofEasyCam _cam;
};

#endif /* defined(__behavior__BehaviorApp__) */
