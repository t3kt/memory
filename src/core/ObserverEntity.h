//
//  ObserverEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef ObserverEntity_h
#define ObserverEntity_h

#include <ofTypes.h>
#include "Entity.h"
#include "State.h"
#include "Common.h"
#include <vector>
#include <iostream>

class OccurrenceEntity;

class ObserverEntity
: public Entity
, public Outputable {
public:
  class Params : public Entity::Params {
  public:
    Params();
    
    ofParameterGroup paramGroup;
    ofParameter<ofVec2f> lifetimeRange;
  };
  
  ObserverEntity(ofVec3f pos, float life, const State& state);
  virtual ~ObserverEntity() {}
  
  void addOccurrence(shared_ptr<OccurrenceEntity> occurrence);
  
  std::vector<shared_ptr<OccurrenceEntity>>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }
  
  float getRemainingLifetimeFraction(const State& state) const;
  
  void draw(const State& state) override;
  
  void output(std::ostream& os) const override;
  
  bool isAlive(const State& state) const override {
    return getRemainingLifetimeFraction(state) > 0;
  }
  
  void handleDeath() override;
  
private:
  const float _startTime;
  const float _totalLifetime;
  std::vector<shared_ptr<OccurrenceEntity>> _connectedOccurrences;
};

#endif /* ObserverEntity_h */
