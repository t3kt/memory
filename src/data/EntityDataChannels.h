//
//  EntityDataChannels.h
//  memory
//
//  Created by tekt on 10/18/16.
//
//

#ifndef EntityDataChannels_h
#define EntityDataChannels_h

#include "../core/WorldObject.h"
#include "../data/DataChannel.h"

class ParticleObject;

enum class EntityChannelId {
  // WorldObject
  ENTITY_ID,
  ENTITY_TYPE,
  ALIVE,
  AGE,
  POSITION,
  COLOR,

  // ParticleObject
  VELOCITY,
  FORCE,
  START_POSITION,
  SIZE,

  // misc
  LIFE_AMOUNT,

  // ObserverEntity
  LIFE_FRACTION = LIFE_AMOUNT,
  SICK,

  // OccurrenceEntity
  ORIGINAL_RADIUS,
  ACTUAL_RADIUS = SIZE,
  AMOUNT_OF_OBSERVATION = LIFE_AMOUNT,
};

class EntityDataChannels {
public:
  EntityDataChannels();

  void addEntity(const ParticleObject& entity);
  void reset();

  DataChannelPtr<EntityType> entityType;
  DataChannelPtr<bool> alive;
  DataChannelPtr<float> age;
  DataChannelPtr<ofVec3f> position;
  DataChannelPtr<ofFloatColor> color;
  DataChannelPtr<float> size;

  const DataChannelList& channels() const { return _channels; }

private:
  template<typename T>
  DataChannelPtr<T> addChannel(EntityChannelId channelId) {
    return _channels.add<T>(enumToString(channelId));
  }

  DataChannelList _channels;

  std::size_t _index;
};

#endif /* EntityDataChannels_h */
