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
#include <vector>
#include <iostream>
#include "ValueSupplier.h"

class OccurrenceEntity;

class ObserverEntity
: public WorldObject {
public:
  class Params : public ::Params {
  public:
    Params();
    
    RandomValueSupplier<float> lifetime;
  };
  
  ObserverEntity(ofVec3f pos, float life, const State& state);
  virtual ~ObserverEntity() override {}
  
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
