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
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include <iostream>
#include "ValueSupplier.h"
#include "State.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  OccurrenceEntity(ofVec3f pos, float radius, const State& state);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _connectedObservers.add(observer);
  }
  
  void removeObserver(ObjectId id) {
    _connectedObservers.remove(id);
  }
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }
  
  float getAmountOfObservation() const { return _amountOfObservation; }

  float getAge(const State& state) const { return state.time - _startTime; }
  
  float originalRadius() const { return _originalRadius; }

  float actualRadius() const { return _actualRadius; }

  const EntityMap<ObserverEntity>& connectedObservers() const {
    return _connectedObservers;
  }

  EntityMap<ObserverEntity>& connectedObservers() {
    return _connectedObservers;
  }

protected:
  std::string typeName() const override { return "OccurrenceEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  void setAmountOfObservation(float amount) {
    _amountOfObservation = amount;
  }

  void setActualRadius(float radius) {
    _actualRadius = radius;
  }
  
  const float _originalRadius;
  float _actualRadius;
  float _startTime;
  float _amountOfObservation;
  EntityMap<ObserverEntity> _connectedObservers;

  friend class OccurrencesController;
};

#endif /* OccurrenceEntity_h */
