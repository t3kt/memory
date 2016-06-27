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
#include <map>
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include "Params.h"
#include <iostream>
#include "ValueSupplier.h"
#include "Bounds.h"
#include "Behavior.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  class Params : public ParticleObject::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    RandomValueSupplier<float> radius;
    ofParameter<ofFloatColor> markerColor;
    ofParameter<ofFloatColor> rangeColor;
    ofParameter<ofFloatColor> connectorColor;
    ofParameter<float> markerSize;
  };
  static shared_ptr<OccurrenceEntity> spawn(const Params& params, const Bounds& bounds);
  
  OccurrenceEntity(ofVec3f pos, float radius, const Params& params);
  virtual ~OccurrenceEntity() {}

  void addBehavior(shared_ptr<Behavior<OccurrenceEntity>> behavior) {
    _behaviors.add(behavior);
  }
  
  void addObserver(shared_ptr<ObserverEntity> observer);
  
  void removeObserver(ObjectId id);
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }
  
  float getAmountOfObservation(const State& state) const;
  
  void update(const State& state) override;
  
  void draw(const State& state) override;
  
  void handleDeath() override;
  
  float originalRadius() const { return _originalRadius; };

protected:
  std::string typeName() const override { return "OccurrenceEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  void recalculateRadius();
  
  const Params& _params;
  const float _originalRadius;
  float _actualRadius;
  std::map<ObjectId, shared_ptr<ObserverEntity>> _connectedObservers;
  BehaviorCollection<OccurrenceEntity> _behaviors;

  friend class OccurrenceObserverAttraction;
};

class OccurrenceObserverAttraction
: public EntityAttraction<OccurrenceEntity, ObserverEntity> {
public:

  OccurrenceObserverAttraction(const Params& params)
  : EntityAttraction<OccurrenceEntity, ObserverEntity>(params) { }

protected:
  std::vector<shared_ptr<ObserverEntity>> getOthers(OccurrenceEntity& occurrence) const override {
    std::vector<shared_ptr<ObserverEntity>> observers;
    for (auto entry : occurrence._connectedObservers) {
      observers.push_back(entry.second);
    }
    return observers;
  }
};

#endif /* OccurrenceEntity_h */
