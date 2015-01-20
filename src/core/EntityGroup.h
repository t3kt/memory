//
//  EntityGroup.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__EntityGroup__
#define __behavior__EntityGroup__

#include <vector>
#include <ofTypes.h>
#include "Entity.h"

template<typename TEntity>
class EntityGroup : public std::vector<ofPtr<TEntity> > {
  
};

#endif /* defined(__behavior__EntityGroup__) */
