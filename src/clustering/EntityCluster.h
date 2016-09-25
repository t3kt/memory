//
//  EntityCluster.h
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#ifndef EntityCluster_h
#define EntityCluster_h

#include "../core/EntityMap.h"

class AnimationObject;
class NavigatorEntity;
class NodeEntity;
class ObserverEntity;
class OccurrenceEntity;
class ParticleObject;
class WorldObject;

class EntityCluster {
public:
  template<typename E>
  void add(std::shared_ptr<E> entity);

  template<typename E>
  EntityMap<E>& getEntityMap();

private:
  template<typename E>
  void addToMap(std::shared_ptr<E> entity) {
    getEntityMap<E>().add(entity);
  }

  EntityMap<NodeEntity> _nodes;
  EntityMap<ObserverEntity> _observers;
  EntityMap<OccurrenceEntity> _occurrences;
};

#endif /* EntityCluster_h */
