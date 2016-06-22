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
#include <vector>
#include "State.h"
#include "AppParameters.h"
#include "OccurrenceEntity.h"
#include "ObserverEntity.h"

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
  std::vector<shared_ptr<OccurrenceEntity>> _occurrences;
  std::vector<shared_ptr<ObserverEntity>> _observers;
};

#endif /* MemoryApp_h */
