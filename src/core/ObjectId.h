//
//  ObjectId.h
//
//  Unique identifiers for objects.
//

#pragma once

using ObjectId = int;

namespace ObjectIds {
  const ObjectId NONE = -1;

  ObjectId next();
}
