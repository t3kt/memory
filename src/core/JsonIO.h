//
//  JsonIO.h
//  memory
//
//  Created by tekt on 6/30/16.
//
//

#ifndef JsonIO_h
#define JsonIO_h

#include <json11.hpp>
#include <ofTypes.h>

using json11::Json;

Json toJsonValue(const ofVec3f& value);

Json toJsonValue(const ofFloatColor& value);

#endif /* JsonIO_h */
