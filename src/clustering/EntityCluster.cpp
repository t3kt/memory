//
//  EntityCluster.cpp
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#include "../clustering/EntityCluster.h"

template<>
EntityMap<NodeEntity>&
EntityCluster::getEntityMap() { return _nodes; }

template<>
EntityMap<ObserverEntity>&
EntityCluster::getEntityMap() { return _observers; }

template<>
EntityMap<OccurrenceEntity>&
EntityCluster::getEntityMap() { return _occurrences; }
