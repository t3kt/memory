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
#include "../core/Common.h"
#include "../core/Connection.h"
#include "../core/EntityMap.h"
#include "../core/ParticleObject.h"
#include "../core/State.h"
#include "../core/ValueSupplier.h"
#include "../core/WorldObject.h"

class OccurrenceEntity;

class ObserverEntity
: public ParticleObject {
public:
  static const auto type = EntityType::OBSERVER;

  static std::shared_ptr<ObserverEntity> createEmpty() {
    return std::shared_ptr<ObserverEntity>(new ObserverEntity());
  }

  ObserverEntity(ofVec3f pos, float decay, const State& state);
  virtual ~ObserverEntity() override {}
  
  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void addObserver(std::shared_ptr<ObserverEntity> observer);

  void removeObserver(ObjectId otherId) {
    _observerConnections.erase(otherId);
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

  float getAge(const State& state) const { return state.time - _startTime; }

  float getDecayRate() const { return _decayRate; }
  void setDecayRate(float decayRate) { _decayRate = decayRate; }

  void detachConnections() override;

  void update(const State& state);

  bool sick() const { return _sick; }
  void setSick(bool sick) { _sick = sick; }

  EntityType entityType() const override { return EntityType::OBSERVER; }

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;
  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  virtual void performActionOnConnected(ObjectPtrRefAction action) override;
  virtual bool hasConnections() const override {
    return !_observerConnections.empty() || !_occurrenceConnections.empty();
  }
  std::string typeName() const override { return "ObserverEntity"; }
protected:
  void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
  virtual void addSerializedRefs(Json::object& obj,
                                 const SerializationContext& context) const override;

private:
  ObserverEntity() { }

  float _startTime;
  float _lifeFraction;
  bool _sick;
  float _decayRate;
  EntityConnectionMap<ObserverEntity> _observerConnections;
  EntityConnectionMap<OccurrenceEntity> _occurrenceConnections;
};

#endif /* ObserverEntity_h */
