//
//  EntityDataChannels.cpp
//  memory
//
//  Created by tekt on 10/18/16.
//
//

#include "../core/Common.h"
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
