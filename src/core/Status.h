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

class Clock;
class ObserversController;
class OccurrencesController;
class AnimationsController;

class StatusInfoController {
public:
  StatusInfoController(const Clock& clock,
                       const ObserversController& observers,
                       const OccurrencesController& occurrences,
                       const AnimationsController& animations);

  void draw();
private:

  const Clock& _clock;
  const ObserversController& _observers;
  const OccurrencesController& _occurrences;
  const AnimationsController& _animations;
  ofxTextAlignTTF _text;
};

#endif /* Status_h */
