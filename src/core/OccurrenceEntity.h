//
//  OccurrenceEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#pragma once

#include <ofTypes.h>
#include <ofxTCommon.h>
#include "../core/Connection.h"
#include "../core/Context.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"
#include "../core/WorldObject.h"

class ObserverEntity;

// An "occurrence" entity which pops into existence in some location
// and becomes connected to any "observers" within range.
// Each occurrence has a range within which it will connect to
// observers, and tracks the state of the connected observers.
// When there are no remaining observers within range, the occurrence
// dies.
class OccurrenceEntity
: public ParticleObject {
public:
  static const auto type = EntityType::OCCURRENCE;

  static std::shared_ptr<OccurrenceEntity> createEmpty(const Context& context) {
    return std::shared_ptr<OccurrenceEntity>(new OccurrenceEntity(context.entityState));
  }

  OccurrenceEntity(ofVec3f pos,
                   float radius,
                   float radiusFraction,
                   const ClockState& state);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _observerConnections.getOrAdd(shared_from_this(),
                                  observer);
  }
  
  void removeObserver(ObjectId id) {
    _observerConnections.erase(id);
  }

  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
    _occurrenceConnections.getOrAdd(shared_from_this(),
                                    occurrence);
  }

  void removeOccurrence(ObjectId id) {
    _occurrenceConnections.erase(id);
  }
  
  bool hasConnectedObservers() const {
    return !_observerConnections.empty();
  }

  void detachConnections() override;
  
  float getAmountOfObservation() const { return _amountOfObservation; }
  
  float originalRadius() const { return _originalRadius; }

  float originalRadiusFraction() const { return _originalRadiusFraction; }

  float actualRadius() const { return _actualRadius; }

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

  void update(const ClockState& state) override;

  EntityType entityType() const override { return EntityType::OCCURRENCE; }

  virtual void deserializeFields(const ofJson& obj,
                                 const SerializationContext& context) override;

  virtual void deserializeRefs(const ofJson& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  virtual void performActionOnConnected(ObjectPtrAction action) override;
  virtual bool hasConnections() const override {
    return !_observerConnections.empty() || !_occurrenceConnections.empty();
  }
  std::string typeName() const override { return "OccurrenceEntity"; }
protected:
  void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(ofJson& obj,
                                   const SerializationContext& context) const override;
  virtual void addSerializedRefs(ofJson& obj,
                                 const SerializationContext& context) const override;
  
private:
  OccurrenceEntity(const ClockState& state)
  : ParticleObject(state) {}

  void setAmountOfObservation(float amount) {
    _amountOfObservation = amount;
  }

  void setActualRadius(float radius) {
    _actualRadius = radius;
  }

  float _originalRadius;
  float _originalRadiusFraction;
  float _actualRadius;
  float _amountOfObservation;
  EntityConnectionMap<ObserverEntity> _observerConnections;
  EntityConnectionMap<OccurrenceEntity> _occurrenceConnections;

  friend class OccurrencesController;
};

