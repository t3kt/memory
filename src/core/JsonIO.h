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

using json11::Json;

class JsonException {
public:
  JsonException(std::string msg) : message(msg) { }
  std::string message;
};

#endif /* JsonIO_h */
