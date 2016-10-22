//
//  Logging.cpp
//  memory
//
//  Created by tekt on 7/25/16.
//
//

#include "../core/Logging.h"

void Logger::log(ofLogLevel level, std::string message) const {
  if (!check(level)) {
    return;
  }
  ofLog(level) << "[" << _label << "] " << message;
}

void Logger::log(ofLogLevel level, Statement statement) const {
  if (!check(level)) {
    return;
  }
  ofLog log(level);
  log << "[" << _label << "] ";
  statement(log);
}
