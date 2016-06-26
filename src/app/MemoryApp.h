//
//  MemoryApp.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef MemoryApp_h
#define MemoryApp_h

#include <ofMain.h>
#include <ofxGui.h>
#include <vector>
#include "State.h"
#include "AppParameters.h"
#include "OccurrenceEntity.h"
#include "ObserverEntity.h"
#include "AnimationObject.h"
#include "ObjectManager.h"
#include "ObserversController.h"
#include "Timing.h"

class MemoryApp : public ofBaseApp {
public:
  void setup() override;
  void update() override;
  void draw() override;
private:
  void spawnOccurrence();
  
  State _state;
  MemoryAppParameters _appParams;
  shared_ptr<ObserversController> _observers;
  ObjectManager<OccurrenceEntity> _occurrences;
  ObjectManager<AnimationObject> _animations;
  TimedActionSet _timedActions;
  ofEasyCam _cam;
  ofxPanel _gui;
};

#endif /* MemoryApp_h */
