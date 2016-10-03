//
//  Connection.h
//  memory
//
//  Created by tekt on 10/3/16.
//
//

#ifndef Connection_h
#define Connection_h

#include <memory>
#include "../core/Common.h"
#include "../core/WorldObject.h"

class AbstractConnection
: public NonCopyable {
public:
  virtual std::shared_ptr<WorldObject> entityBaseA() = 0;
  virtual std::shared_ptr<WorldObject> entityBaseB() = 0;
};

template<typename Ta, typename Tb>
class EntityConnection
: public AbstractConnection {
public:
  EntityConnection(std::shared_ptr<Ta>& a,
                   std::shared_ptr<Tb>& b)
  : _a(a)
  , _b(b) { }

  std::shared_ptr<WorldObject> entityBaseA() override {
    return _a;
  }

  std::shared_ptr<WorldObject> entityBaseB() override {
    return _b;
  }

  std::shared_ptr<Ta>& a() { return _a; }
  const std::shared_ptr<Ta>& a() const { return _a; }

  std::shared_ptr<Tb>& b() { return _b; }
  const std::shared_ptr<Tb>& b() const { return _b; }

private:
  std::shared_ptr<Ta> _a;
  std::shared_ptr<Tb> _b;
};

#endif /* Connection_h */
