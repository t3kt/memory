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
#include "State.h"
#include "Colors.h"
#include "ObjectManager.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  class Params : public ParticleObject::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;
    
    RandomValueSupplier<float> radius;
    TParam<float> rangeFadeIn;
    TParam<float> markerSize;
  };
  static shared_ptr<OccurrenceEntity> spawn(const Params& params, const Bounds& bounds, const State& state, const ColorTheme& colors);
  
  OccurrenceEntity(ofVec3f pos, float radius, const Params& params, const State& state, const ColorTheme& colors);
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
  
  void handleDeath() override;
  
  float originalRadius() const { return _originalRadius; }

  float actualRadius() const { return _actualRadius; }

  const std::map<ObjectId, shared_ptr<ObserverEntity>>& connectedObservers() const {
    return _connectedObservers;
  }

  float getFadeInPercent(const State& state) const;

protected:
  std::string typeName() const override { return "OccurrenceEntity"; }
  void outputFields(std::ostream& os) const override;
  
private:
  void recalculateRadius();
  
  const Params& _params;
  const ColorTheme& _colors;
  const float _originalRadius;
  float _actualRadius;
  float _startTime;
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
