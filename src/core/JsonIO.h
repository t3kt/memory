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
#include <iostream>

using json11::Json;

class JsonException {
public:
  JsonException(std::string msg) : message(msg) { }
  std::string message;
};

void prettyPrintJsonToStream(const Json& value, std::ostream& os);

std::string prettyPrintJsonToString(const Json& value);

#endif /* JsonIO_h */
