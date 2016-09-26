//
//  EntityGroup.h
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#ifndef EntityGroup_h
#define EntityGroup_h

#include "../core/Common.h"
#include "../core/EntityMap.h"
#include "../core/ObjectId.h"
#include "../core/ParticleObject.h"

class NodeEntity;
class ObserverEntity;
class OccurrenceEntity;
class ParticleObject;

class EntityGroup {
public:
  EntityGroup();

  template<typename E>
  EntityMap<E>& getEntities();

  template<typename E>
  bool add(std::shared_ptr<E> entity) {
    return getEntities<E>().add(entity);
  }

  template<typename E, typename A>
  void performEntityAction(A action) {
    getEntities<E>().performAction(action);
  }

  void performAllEntityAction(PtrAction<ParticleObject> action);

  std::size_t size() const {
    return _nodes.size() + _observers.size() + _occurrences.size();
  }

  bool containsId(ObjectId objId) const {
    return _nodes.containsId(objId) || _observers.containsId(objId) || _occurrences.containsId(objId);
  }

  void cullDeadObjects();

  void clear();
private:
  bool addSingle(ParticlePtr entity);
  bool addWithConnections(ParticlePtr fromEntity);
  void addConnections(ParticlePtr fromEntity,
                      std::size_t depthLimit = -1);

  ObjectId _id;
  EntityMap<NodeEntity> _nodes;
  EntityMap<ObserverEntity> _observers;
  EntityMap<OccurrenceEntity> _occurrences;
};

#endif /* EntityGroup_h */
