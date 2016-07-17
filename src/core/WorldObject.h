//
//  WorldObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef WorldObject_h
#define WorldObject_h

#include "Common.h"
#include <iostream>
#include <map>
#include <memory>

typedef long ObjectId;

enum class EntityType {
  ANIMATION,
  OBSERVER,
  OCCURRENCE,
};

template<typename T>
EntityType getEntityType();

class WorldObject : public Outputable {
public:
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId id;
  
  void output(std::ostream& os) const override;

  bool alive() const { return _alive; }

  void kill() { _alive = false; }
  
  virtual bool visible() const { return this->alive(); }

  virtual EntityType entityType() const = 0;
protected:
  virtual std::string typeName() const;
  virtual void outputFields(std::ostream& os) const;
private:
  bool _alive;
};

template<typename E>
class EntityMap {
public:
  using EntityPtr = std::shared_ptr<E>;
  using Storage = std::map<ObjectId, EntityPtr>;
  using iterator = typename Storage::iterator;
  using const_iterator = typename Storage::const_iterator;

  void add(EntityPtr entity) { _map[entity->id] = entity; }

  std::size_t erase(ObjectId id) { return _map.erase(id); }

  std::size_t size() const { return _map.size(); }

  bool empty() const { return _map.empty(); }

  iterator begin() { return _map.begin(); }
  iterator end() { return _map.end(); }
  const_iterator begin() const { return _map.begin(); }
  const_iterator end() const { return _map.end(); }
private:
  Storage _map;
};

#endif /* WorldObject_h */
