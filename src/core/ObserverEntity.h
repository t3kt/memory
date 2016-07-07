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
#include "Params.h"
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include <vector>
#include <iostream>
#include "ValueSupplier.h"
#include "Bounds.h"

class OccurrenceEntity;

class ObserverEntity
: public ParticleObject {
public:
  class Params : public ParticleObject::Params {
  public:
    Params();
    
    RandomValueSupplier<float> lifetime;
  };
  
  static shared_ptr<ObserverEntity> spawn(const Params& params, const Bounds& bounds, const State& state);
  
  ObserverEntity(ofVec3f pos, float life, const Params& params, const State& state);
  virtual ~ObserverEntity() override {}
  
  void addOccurrence(shared_ptr<OccurrenceEntity> occurrence);
  
  EntityMap<OccurrenceEntity>& getConnectedOccurrences() {
    return _connectedOccurrences;
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
};

#endif /* ObserverEntity_h */
