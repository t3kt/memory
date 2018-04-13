//
//  ObjectId.h
//
//  Unique identifiers for objects.
//

#pragma once

// A application-wide unique entity identifier.
using ObjectId = int;

namespace ObjectIds {
  const ObjectId NONE = -1;

  ObjectId next();
}
