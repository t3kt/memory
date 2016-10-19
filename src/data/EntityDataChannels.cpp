//
//  EntityDataChannels.cpp
//  memory
//
//  Created by tekt on 10/18/16.
//
//

#include "../core/Common.h"
#include "../core/ParticleObject.h"
#include "../data/EntityDataChannels.h"

EnumTypeInfo<EntityChannelId> EntityChannelIdType({
  {"id", EntityChannelId::ENTITY_ID},
  {"type", EntityChannelId::ENTITY_TYPE},
  {"alive", EntityChannelId::ALIVE},
  {"age", EntityChannelId::AGE},
  {"pos", EntityChannelId::POSITION},
  {"col", EntityChannelId::COLOR},
  {"vel", EntityChannelId::VELOCITY},
  {"frc", EntityChannelId::FORCE},
  {"startpos", EntityChannelId::START_POSITION},
  {"size", EntityChannelId::SIZE},
  {"lifeamt", EntityChannelId::LIFE_AMOUNT},
  {"sick", EntityChannelId::SICK},
  {"origrad", EntityChannelId::ORIGINAL_RADIUS},
});

template<>
const EnumTypeInfo<EntityChannelId>& getEnumInfo() {
  return EntityChannelIdType;
}

std::ostream& operator<<(std::ostream& os,
                         const EntityChannelId& action) {
  return os << enumToString(action);
}

EntityDataChannels::EntityDataChannels()
: entityType(enumToString(EntityChannelId::ENTITY_TYPE))
, alive(enumToString(EntityChannelId::ALIVE))
, age(enumToString(EntityChannelId::AGE))
, position(enumToString(EntityChannelId::POSITION))
, color(enumToString(EntityChannelId::COLOR))
, size(enumToString(EntityChannelId::SIZE)) { }

void EntityDataChannels::reset() {
  entityType.wipe();
  alive.wipe();
  age.wipe();
  position.wipe();
  color.wipe();
  size.wipe();
  _index = 0;
}

void EntityDataChannels::addEntity(const ParticleObject &entity) {
  entityType.setSlice(_index, entity.entityType());
  alive.setSlice(_index, entity.alive());
  age.setSlice(_index, entity.age().get());
  position.setSlice(_index, entity.position());
  color.setSlice(_index, entity.color());
  size.setSlice(_index, entity.size());
  _index++;
}
