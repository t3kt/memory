//
//  NavigatorEntity.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef NavigatorEntity_h
#define NavigatorEntity_h

#include <memory>
#include "ParticleObject.h"

class Context;
class NavigatorState;
using NavigatorStatePtr = std::shared_ptr<NavigatorState>;

class NavigatorEntity
: public ParticleObject {
public:
  static std::shared_ptr<NavigatorEntity> createEmpty() {
    // unsupported
    return std::shared_ptr<NavigatorEntity>();
  }

  NavigatorEntity(NavigatorStatePtr prevState,
                  Context& context);

  const ofVec3f& prevPosition() const;
  const ofVec3f& nextPosition() const;

  NavigatorState* prevState() { return _prevState.get(); }
  NavigatorState* nextState() { return _nextState.get(); }

  const ofVec3f& targetPoint() const;

  float lastChangeTime() const { return _lastChangeTime; }

  EntityType entityType() const override { return EntityType::NAVIGATOR; }

  void updateNextState(Context& context);
  void reachNextState(Context& context);

  void setPosition(const ofVec3f& pos) { _position = pos; }

  bool stateAlive() const;

  void fillInfo(Info& info) const override;

  std::string typeName() const override { return "NavigatorEntity"; }
protected:
  void outputFields(std::ostream& os) const override;

private:
  NavigatorStatePtr _prevState;
  NavigatorStatePtr _nextState;
  float _lastChangeTime;
};

template<>
EntityType getEntityType<NavigatorEntity>() { return EntityType::NAVIGATOR; }

#endif /* NavigatorEntity_h */
