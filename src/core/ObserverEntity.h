//
//  ObserverEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#pragma once

#include <ofTypes.h>
#include "../core/Common.h"
#include "../core/Connection.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"
#include "../core/WorldObject.h"

class OccurrenceEntity;

// An "observer" entity, which moves around and experiences
// "occurrences" which connect it to other observers.
// Observers have a lifespan, after which they die.
class ObserverEntity
: public ParticleObject {
public:
  static const auto type = EntityType::OBSERVER;

  static std::shared_ptr<ObserverEntity> createEmpty(const Context& context);

  ObserverEntity(ofVec3f pos, float decay, const ClockState& state);
  virtual ~ObserverEntity() override {}
  
  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void addObserver(std::shared_ptr<ObserverEntity> observer);

  void removeObserver(ObjectId otherId) {
    _observerConnections.erase(otherId);
  }

  void removeOccurrence(ObjectId otherId) {
    _occurrenceConnections.erase(otherId);
  }

  EntityConnectionMap<ObserverEntity>&
  getObserverConnections() {
    return _observerConnections;
  }

  const EntityConnectionMap<ObserverEntity>&
  getObserverConnections() const {
    return _observerConnections;
  }

  EntityConnectionMap<OccurrenceEntity>&
  getOccurrenceConnections() {
    return _occurrenceConnections;
  }

  const EntityConnectionMap<OccurrenceEntity>&
  getOccurrenceConnections() const {
    return _occurrenceConnections;
  }
  
  float getRemainingLifetimeFraction() const { return _lifeFraction; }

  float getDecayRate() const { return _decayRate; }
  void setDecayRate(float decayRate) { _decayRate = decayRate; }

  void detachConnections() override;

  void update(const ClockState& state) override;

  bool sick() const { return _sick; }
  void setSick(bool sick) { _sick = sick; }

  EntityType entityType() const override { return EntityType::OBSERVER; }

  virtual void deserializeFields(const ofJson& obj,
                                 const SerializationContext& context) override;
  virtual void deserializeRefs(const ofJson& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  virtual void performActionOnConnected(ObjectPtrAction action) override;
  virtual bool hasConnections() const override {
    return !_observerConnections.empty() || !_occurrenceConnections.empty();
  }
  std::string typeName() const override { return "ObserverEntity"; }
protected:
  void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(ofJson& obj,
                                   const SerializationContext& context) const override;
  virtual void addSerializedRefs(ofJson& obj,
                                 const SerializationContext& context) const override;

private:
  ObserverEntity(const ClockState& state)
  : ParticleObject(state) {}

  float _lifeFraction;
  bool _sick;
  float _decayRate;
  EntityConnectionMap<ObserverEntity> _observerConnections;
  EntityConnectionMap<OccurrenceEntity> _occurrenceConnections;
};

