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
  
  void addOccurrence(shared_ptr<OccurrenceEntity> occurrence);

  void addObserver(shared_ptr<ObserverEntity> observer) {
    _connectedObservers.add(observer);
  }

  void removeObserver(ObjectId otherId) {
    _connectedObservers.remove(otherId);
  }
  
  EntityMap<OccurrenceEntity>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }

  EntityMap<ObserverEntity>& getConnectedObservers() {
    return _connectedObservers;
  }
  
  float getRemainingLifetimeFraction() const { return _lifeFraction; }
  
  void update(const State& state) override;
  
  void handleDeath() override;
  
  float lifetime() const { return _totalLifetime; };

protected:
  std::string typeName() const override { return "ObserverEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  const float _totalLifetime;
  float _lifeFraction;
  EntityMap<OccurrenceEntity> _connectedOccurrences;
  EntityMap<ObserverEntity> _connectedObservers;
};

#endif /* ObserverEntity_h */
