//
//  NodeEntity.h
//  memory
//
//  Created by tekt on 9/18/16.
//
//

#ifndef NodeEntity_h
#define NodeEntity_h

#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/ParticleObject.h"
#include "../core/WorldObject.h"

class NodeEntity
: public ParticleObject {
public:
  static const auto type = EntityType::NODE;

  NodeEntity(ofVec3f pos) : ParticleObject(pos) { }

  std::string typeName() const override { return "Node"; }

  EntityType entityType() const override { return EntityType::NODE; }
};

#endif /* NodeEntity_h */
