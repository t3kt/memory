//
//  EntityScanner.h
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#ifndef EntityScanner_h
#define EntityScanner_h

#include <functional>
#include <memory>
#include <unordered_set>
#include "../core/ObjectId.h"
#include "../core/WorldObject.h"

class WorldObject;

class ScanResult {
public:
  static ScanResult accept() { return ScanResult(true); }
  static ScanResult reject() { return ScanResult(false); }

  ScanResult& andCancel() {
    _cancel = true;
    return *this;
  }

  ScanResult& andIncludeChildren() {
    _children = true;
    return *this;
  }

  bool isCancel() const { return _cancel; }
  bool isAccept() const { return _accept; }
  bool includeChildren() const { return _children; }
private:
  ScanResult(bool accept)
  : _accept(accept) { }

  bool _accept;
  bool _children;
  bool _cancel;
};

using ScanTest =
std::function<ScanResult(std::shared_ptr<WorldObject>)>;

using ScanReceiver =
std::function<bool(std::shared_ptr<WorldObject>)>;

class EntityScanPolicy {
public:
  bool duplicates = false;
  int maxDepth = -1;
  int maxTotal = -1;
  bool includeNodes = false;
  bool includeObservers = false;
  bool includeOccurrences = false;
  ScanTest tester;

  bool acceptsType(EntityType type) const {
    switch (type) {
      case EntityType::NODE:
        return includeNodes;
      case EntityType::OBSERVER:
        return includeObservers;
      case EntityType::OCCURRENCE:
        return includeOccurrences;
      default:
        return false;
    }
  }
};

/*
 scan all the entities in some collection
 scan all the entities in some collection plus their connections
*/

class EntityScanner
: public NonCopyable {
public:

  EntityScanner(EntityScanPolicy policy,
                ScanReceiver receiver)
  : _policy(policy)
  , _receiver(receiver) { }

  template<typename T>
  bool scan(T target) {
    auto result = scan(target, 0);
    return result.isAccept();
  }
private:
  template<typename T>
  ScanResult scan(T target,
                  int currentDepth);

//  template<typename T>
//  bool checkPolicy(std::shared_ptr<T>& entity);

  ScanReceiver _receiver;
  const EntityScanPolicy _policy;
  int _total;
  std::unordered_set<ObjectId> _scannedIds;
};

#endif /* EntityScanner_h */
