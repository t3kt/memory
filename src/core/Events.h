//
//  Events.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef Events_h
#define Events_h

#include "Common.h"

template<typename T>
class EntityEventArgs : public Outputable {
public:
  EntityEventArgs(shared_ptr<T> entity) : _entity(entity) {}
  
  void output(std::ostream& os) const override {
    os << "EntityEventArgs{"
        << *_entity
        << "}";
  }
private:
  shared_ptr<T> _entity;
};

#endif /* Events_h */
