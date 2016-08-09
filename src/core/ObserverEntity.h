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
#include "Common.h"
#include "EntityMap.h"
#include "ParticleObject.h"
#include "ValueSupplier.h"
#include "WorldObject.h"

class OccurrenceEntity;

class ObserverEntity
: public ParticleObject {
public:
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

  float lifetime() const { return _totalLifetime; };

  EntityType entityType() const override { return EntityType::OBSERVER; }

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;
  virtual void deserializeRefs(const Json& obj,
                               SerializationContext& context) override;

  virtual void fillInfo(Info& info) const override;
  virtual void performActionOnConnected(ObjectPtrAction action) override;
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
  EntityMap<OccurrenceEntity> _connectedOccurrences;
  EntityMap<ObserverEntity> _connectedObservers;
};

template<>
EntityType getEntityType<ObserverEntity>() { return EntityType::OBSERVER; }

#endif /* ObserverEntity_h */
