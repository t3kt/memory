//
//  EntityScanner.cpp
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#include "../core/Context.h"
#include "../core/EntityMap.h"
#include "../core/NodeEntity.h"
#include "../core/ObjectManager.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../graph/EntityScanner.h"

template<>
ScanResult
EntityScanner::scan(std::shared_ptr<ObserverEntity>& entity,
                    int currentDepth) {
  if (_policy.maxTotal != -1 && _total > _policy.maxTotal) {
    return ScanResult::reject();
  }
  if (_policy.maxDepth != -1 && currentDepth > _policy.maxDepth) {
    return ScanResult::reject();
  }
  auto id = entity->id();
  auto type = entity->entityType();
  if (!_policy.duplicates && _scannedIds.find(id) != _scannedIds.end()) {
    return ScanResult::reject();
  }
  if (!_policy.acceptsType(type)) {
    return ScanResult::reject();
  }
  bool cancel = false;
  bool scanChildren = false;
  if (_policy.maxDepth != -1 && (currentDepth + 1) > _policy.maxDepth) {
    scanChildren = false;
  }
  if (_policy.tester) {
    auto result = _policy.tester(entity);
  }
  //...
  
  return ScanResult::reject();
}
