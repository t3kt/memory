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
#include "Behavior.h"
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include <vector>
#include <iostream>
#include "ValueSupplier.h"
#include "Behavior.h"
#include "Bounds.h"

class OccurrenceEntity;

class ObserverEntity
: public ParticleObject {
public:
  class Params : public ParticleObject::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    RandomValueSupplier<float> lifetime;
    ofParameter<ofFloatColor> color;
    ofParameter<float> drawRadius;
  };
  
  static shared_ptr<ObserverEntity> spawn(const Params& params, const Bounds& bounds, const State& state);
  
  ObserverEntity(ofVec3f pos, float life, const Params& params, const State& state);
  virtual ~ObserverEntity() override {}

  void addBehavior(shared_ptr<Behavior<ObserverEntity>> behavior) {
    _behaviors.add(behavior);
  }
  
  void addOccurrence(shared_ptr<OccurrenceEntity> occurrence);
  
  std::vector<shared_ptr<OccurrenceEntity>>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }
  
  float getRemainingLifetimeFraction() const { return _lifeFraction; }
  
  void update(const State& state) override;
  
  void draw(const State& state) override;
  
  void handleDeath() override;
  
  float lifetime() const { return _totalLifetime; };

protected:
  std::string typeName() const override { return "ObserverEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  const Params& _params;
  const float _startTime;
  const float _totalLifetime;
  float _lifeFraction;
  std::vector<shared_ptr<OccurrenceEntity>> _connectedOccurrences;
  BehaviorCollection<ObserverEntity> _behaviors;

  friend class ObserverOccurrenceAttraction;
};

class ObserverOccurrenceAttraction
: public EntityAttraction<ObserverEntity, OccurrenceEntity> {
public:

  ObserverOccurrenceAttraction(const AbstractEntityAttraction::Params& params)
  : EntityAttraction<ObserverEntity, OccurrenceEntity>(params) { }

protected:
  std::vector<shared_ptr<OccurrenceEntity>> getOthers(ObserverEntity& observer) const override {
    return observer._connectedOccurrences;
  }
};

#endif /* ObserverEntity_h */
