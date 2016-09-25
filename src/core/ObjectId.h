//
//  ObjectId.h
//  memory
//
//  Created by tekt on 9/24/16.
//
//

#ifndef ObjectId_h
#define ObjectId_h

using ObjectId = int;

namespace ObjectIds {
  const ObjectId NONE = -1;

  ObjectId next();
}

#endif /* ObjectId_h */
