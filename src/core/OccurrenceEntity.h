//
//  OccurrenceEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef OccurrenceEntity_h
#define OccurrenceEntity_h

#include <iostream>
#include <ofTypes.h>
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/EntityMap.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"
#include "../core/ValueSupplier.h"
#include "../core/WorldObject.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  static const auto type = EntityType::OCCURRENCE;

  static std::shared_ptr<OccurrenceEntity> createEmpty() {
    return std::shared_ptr<OccurrenceEntity>(new OccurrenceEntity());
  }

  OccurrenceEntity(ofVec3f pos,
                   float radius,
                   float radiusFraction,
                   const State& state);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _connectedObservers.add(observer);
  }
  
  void removeObserver(ObjectId id) {
    _connectedObservers.erase(id);
  }

  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
    _connectedOccurrences.add(occurrence);
  }

  void removeOccurrence(ObjectId id) {
    _connectedOccurrences.erase(id);
  }
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }

  void detachConnections() override;
  
  float getAmountOfObservation() const { return _amountOfObservation; }

  float getAge(const State& state) const { return state.time - _startTime; }
  
  float originalRadius() const { return _originalRadius; }

  float originalRadiusFraction() const { return _originalRadiusFraction; }

  float actualRadius() const { return _actualRadius; }

  const EntityMap<ObserverEntity>& getConnectedObservers() const {
    return _connectedObservers;
  }

  EntityMap<ObserverEntity>& getConnectedObservers() {
    return _connectedObservers;
  }

  const EntityMap<OccurrenceEntity>& getConnectedOccurrences() const {
    return _connectedOccurrences;
  }

  EntityMap<OccurrenceEntity>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }

  void update();

  EntityType entityType() const override { return EntityType::OCCURRENCE; }

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;

  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  virtual void performActionOnConnected(ObjectPtrRefAction action) override;
  virtual bool hasConnections() const override {
    return !_connectedObservers.empty() || !_connectedOccurrences.empty();
  }
  std::string typeName() const override { return "OccurrenceEntity"; }
protected:
  void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
  virtual void addSerializedRefs(Json::object& obj,
                                 const SerializationContext& context) const override;
  
private:
  OccurrenceEntity() { }

  void setAmountOfObservation(float amount) {
    _amountOfObservation = amount;
  }

  void setActualRadius(float radius) {
    _actualRadius = radius;
  }

  float _originalRadius;
  float _originalRadiusFraction;
  float _actualRadius;
  float _startTime;
  float _amountOfObservation;
  EntityMap<ObserverEntity> _connectedObservers;
  EntityMap<OccurrenceEntity> _connectedOccurrences;

  friend class OccurrencesController;
};

#endif /* OccurrenceEntity_h */
