//
//  OccurrenceEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef OccurrenceEntity_h
#define OccurrenceEntity_h

#include <ofTypes.h>
#include "Entity.h"
#include <map>
#include "Common.h"
#include <iostream>

class ObserverEntity;

class OccurrenceEntity
: public Entity
, public Outputable {
public:
  class Params : public Entity::Params {
  public:
    Params();
    
    ofParameter<ofVec2f> radiusRange;
  };
  OccurrenceEntity(ofVec3f pos, float radius);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(shared_ptr<ObserverEntity> observer);
  
  void removeObserver(EntityId id);
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }
  
  float getAmountOfObservation(const State& state) const;
  
  void draw(const State& state) override;
  
  void output(std::ostream& os) const override;
  
  bool isAlive(const State& state) const override {
    return hasConnectedObservers();
  }
  
private:
  void recalculateRadius();
  
  const float _originalRadius;
  float _actualRadius;
  std::map<EntityId, shared_ptr<ObserverEntity>> _connectedObservers;
};

#endif /* OccurrenceEntity_h */
