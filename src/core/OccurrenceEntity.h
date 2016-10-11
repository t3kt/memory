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
#include "../core/Common.h"
#include "../core/Connection.h"
#include "../core/Context.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"
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
                   const ClockState& state);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _observerConnections.getOrAdd(observer);
  }
  
  void removeObserver(ObjectId id) {
    _observerConnections.erase(id);
  }

  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
    _occurrenceConnections.getOrAdd(occurrence);
  }

  void removeOccurrence(ObjectId id) {
    _occurrenceConnections.erase(id);
  }
  
  bool hasConnectedObservers() const {
    return !_observerConnections.empty();
  }

  void detachConnections() override;
  
  float getAmountOfObservation() const { return _amountOfObservation; }

  float getAge(const ClockState& state) const { return state.localTime - _startTime; }
  
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

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;

  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  virtual void performActionOnConnected(ObjectPtrAction action) override;
  virtual bool hasConnections() const override {
    return !_observerConnections.empty() || !_occurrenceConnections.empty();
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
  EntityConnectionMap<ObserverEntity> _observerConnections;
  EntityConnectionMap<OccurrenceEntity> _occurrenceConnections;

  friend class OccurrencesController;
};

#endif /* OccurrenceEntity_h */
