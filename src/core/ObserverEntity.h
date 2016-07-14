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
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include <iostream>
#include "ValueSupplier.h"

class OccurrenceEntity;

class ObserverEntity
: public ParticleObject {
public:
  ObserverEntity(ofVec3f pos, float life, const State& state);
  virtual ~ObserverEntity() override {}
  
  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _connectedObservers.add(observer);
  }

  void removeObserver(ObjectId otherId) {
    _connectedObservers.erase(otherId);
  }
  
  EntityMap<OccurrenceEntity>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }

  EntityMap<ObserverEntity>& getConnectedObservers() {
    return _connectedObservers;
  }
  
  float getRemainingLifetimeFraction() const { return _lifeFraction; }

  float getAge(const State& state) const { return state.time - _startTime; }

  void detachConnections();

  void update(const State& state);

  float lifetime() const { return _totalLifetime; };

protected:
  std::string typeName() const override { return "ObserverEntity"; }
  void outputFields(std::ostream& os) const override;

private:
  float _startTime;
  const float _totalLifetime;
  float _lifeFraction;
  EntityMap<OccurrenceEntity> _connectedOccurrences;
  EntityMap<ObserverEntity> _connectedObservers;
};

#endif /* ObserverEntity_h */
