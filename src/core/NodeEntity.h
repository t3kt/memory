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
#include "../core/State.h"
#include "../core/WorldObject.h"

class NodeEntity
: public ParticleObject {
public:
  static const auto type = EntityType::NODE;

  NodeEntity() {}
  virtual ~NodeEntity() {}

  virtual void update(const State& state) {}

  std::string typeName() const override { return "Node"; }
private:
};

#endif /* NodeEntity_h */
