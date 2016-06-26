//
//  AnimationObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef AnimationObject_h
#define AnimationObject_h

#include "WorldObject.h"
#include "ObjectManager.h"
#include "Timing.h"
#include "State.h"

class AnimationObject : public WorldObject {
public:
  virtual void draw(const State& state) override = 0;
  virtual void output(std::ostream& os) const override;
  
  void show() { _visible = true; }
  void hide() { _visible = false; }
  
  virtual bool visible() const override {
    return alive() && _visible;
  }
  
  shared_ptr<DurationAction>
  createUpdaterAction(float time, ObjectManager<AnimationObject>& animations);
protected:
  inline float percentage() const { return _percentage; }
private:
  bool _visible;
  float _delay;
  float _duration;
  float _percentage;
  
  friend class AnimationUpdater;
};

#endif /* AnimationObject_h */
