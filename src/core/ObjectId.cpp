//
//  ObjectId.cpp
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#include "../core/ObjectId.h"

namespace ObjectIds {
  ObjectId next() {
    static ObjectId lastId = 0;
    return ++lastId;
  }
}
