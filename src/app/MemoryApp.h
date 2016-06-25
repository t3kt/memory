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
#include "ObjectManager.h"

class MemoryApp : public ofBaseApp {
public:
  void setup() override;
  void update() override;
  void draw() override;
private:
  void spawnObserver();
  void spawnOccurrence();
  
  State _state;
  MemoryAppParameters _appParams;
  ObjectManager<OccurrenceEntity> _occurrences;
  ObjectManager<ObserverEntity> _observers;
  ofEasyCam _cam;
  ofxPanel _gui;
};

#endif /* MemoryApp_h */
