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

ofxTCommon::EnumTypeInfo<EntityChannelId> EntityChannelIdType({
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
const EnumTypeInfo<EntityChannelId>& ofxTCommon::getEnumInfo() {
  return EntityChannelIdType;
}

std::ostream& operator<<(std::ostream& os,
                         const EntityChannelId& action) {
  return os << ofxTCommon::enumToString(action);
}

EntityDataChannels::EntityDataChannels() {
  entityType =
  addChannel<EntityType>(EntityChannelId::ENTITY_TYPE);
  alive =
  addChannel<bool>(EntityChannelId::ALIVE);
  age =
  addChannel<float>(EntityChannelId::AGE);
  position =
  addChannel<ofVec3f>(EntityChannelId::POSITION);
  color =
  addChannel<ofFloatColor>(EntityChannelId::COLOR);
  size =
  addChannel<float>(EntityChannelId::SIZE);
}

void EntityDataChannels::reset() {
  for (auto& channel : _channels) {
    channel->wipe();
  }
  _index = 0;
}

void EntityDataChannels::addEntity(const ParticleObject &entity) {
  entityType->setSlice(_index, entity.entityType());
  alive->setSlice(_index, entity.alive());
  age->setSlice(_index, entity.age().get());
  position->setSlice(_index, entity.position());
  color->setSlice(_index, entity.color());
  size->setSlice(_index, entity.size());
  _index++;
}
