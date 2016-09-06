//
//  ObserverEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef ObserverEntity_h
#define ObserverEntity_h

#include <iostream>
#include <ofTypes.h>
#include "../core/Common.h"
#include "../core/EntityMap.h"
#include "../core/ParticleObject.h"
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

  ObserverEntity(ofVec3f pos, float life, const State& state);
  virtual ~ObserverEntity() override {}
  
  void addOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void addObserver(std::shared_ptr<ObserverEntity> observer) {
    _connectedObservers.add(observer);
  }

  void removeObserver(ObjectId otherId) {
    _connectedObservers.erase(otherId);
  }
  
  EntityMap<OccurrenceEntity>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }

  EntityMap<ObserverEntity>& getConnectedObservers() {
    return _connectedObservers;
  }
  
  float getRemainingLifetimeFraction() const { return _lifeFraction; }

  float getAge(const State& state) const { return state.time - _startTime; }

  void detachConnections();

  void update(const State& state);

  float remainingLife() const { return _remainingLife; }

  void modifyRemainingLife(float multiplier) {
    _remainingLife *= multiplier;
  }
  void capRemainingLifetime(float maxLifetime) {
    if (_remainingLife > maxLifetime) {
      _remainingLife = maxLifetime;
    }
  }

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
    return !_connectedObservers.empty() || !_connectedOccurrences.empty();
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
  float _totalLifetime;
  float _lifeFraction;
  float _remainingLife;
  bool _sick;
  EntityMap<OccurrenceEntity> _connectedOccurrences;
  EntityMap<ObserverEntity> _connectedObservers;
};

#endif /* ObserverEntity_h */
