//
//  OccurrenceEntity.hpp
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef OccurrenceEntity_h
#define OccurrenceEntity_h

#include <ofTypes.h>
#include "Entity.h"
#include <vector>

class ObserverEntity;

class OccurrenceEntity : public Entity {
public:
  OccurrenceEntity(ofVec3f pos, float radius);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(shared_ptr<ObserverEntity> observer);
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }
  
  float getAmountOfObservation(const State& state) const;
  
  virtual void draw(State& state) override;
  
private:
  const float _originalRadius;
  float _actualRadius;
  std::vector<shared_ptr<ObserverEntity>> _connectedObservers;
};

#endif /* OccurrenceEntity_hpp */
