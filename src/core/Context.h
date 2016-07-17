//
//  Context.h
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#ifndef Context_h
#define Context_h

#include "Common.h"
#include "ObjectManager.h"
#include "State.h"

class AnimationObject;
class ObserverEntity;
class OccurrenceEntity;

class Context
: public NonCopyable {
public:
  State state;
  ObjectManager<AnimationObject> animations;
  ObjectManager<ObserverEntity> observers;
  ObjectManager<OccurrenceEntity> occurrences;
};

#endif /* Context_h */
