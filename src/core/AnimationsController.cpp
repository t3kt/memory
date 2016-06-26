//
//  AnimationsController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include "AnimationsController.h"
#include <ofMain.h>
#include <iostream>

void AnimationsController::addAnimation(shared_ptr<AnimationObject> animation, const State& state) {
  std::cout << "Adding animation: " << *animation << std::endl;
  
  _animations.add(animation);
  auto updater = animation->createUpdaterAction(state.time, _animations);
  addTimedAction(updater);
}

void AnimationsController::addTimedAction(shared_ptr<TimedAction> action) {
  _timedActions.add(action);
}

void AnimationsController::update(const State &state) {
  _animations.update(state);
  _timedActions.update(state.time);
  _animations.cullDeadObjects([&](shared_ptr<AnimationObject> animation) {
    std::cout << "Animation ended: " << *animation << std::endl;
  });
}

void AnimationsController::draw(const State &state) {
  _animations.draw(state);
}

class ObserverDiedAnimation : public AnimationObject {
public:
  ObserverDiedAnimation(const ObserverEntity& observer)
  : AnimationObject(0, 5)
  , _position(observer.position) {}
  
  void draw(const State& state) override {
    ofPushMatrix();
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofFloatColor(0, 0, 0.5, ofMap(percentage(), 0, 1, 0.5, 0)));
    ofDrawSphere(_position, ofMap(percentage(), 0, 1, 0, 0.5));
    
    ofPopStyle();
    ofPopMatrix();
  }
  
  void output(std::ostream& os) const override {
    os << "ObserverDiedAnimation{id: " << id
        << "position: " << _position
        << "}";
  }
private:
  const ofVec3f _position;
    };
    
class OccurrenceDiedAnimation : public AnimationObject {
public:
  OccurrenceDiedAnimation(const OccurrenceEntity& occurrence)
  : AnimationObject(0, 5)
  , _position(occurrence.position) {}
  
  void draw(const State& state) override {
    ofPushMatrix();
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetColor(ofFloatColor(0, 0.5, 0.2, ofMap(percentage(), 0, 1, 0.5, 0)));
    ofDrawSphere(_position, ofMap(percentage(), 0, 1, 0, 0.5));
    
    ofPopStyle();
    ofPopMatrix();
  }
  
  void output(std::ostream& os) const override {
    os << "OccurrenceDiedAnimation{id: " << id
    << "position: " << _position
    << "}";
  }
private:
  const ofVec3f _position;
};

void AnimationsController::attachTo(ObserversController &observers) {
  observers.observerDied += [&](ObserverEventArgs e) {
    auto observer = e.entity();
    addAnimation(std::make_shared<ObserverDiedAnimation>(*observer), e.state);
  };
  //...
}

void AnimationsController::attachTo(OccurrencesController &occurrences) {
  occurrences.occurrenceDied += [&](OccurrenceEventArgs e) {
    auto occurrence = e.entity();
    addAnimation(std::make_shared<OccurrenceDiedAnimation>(*occurrence), e.state);
  };
  //...
}
