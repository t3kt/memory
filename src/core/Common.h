//
//  Common.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__Common__
#define __behavior__Common__

#include <ofVec3f.h>
#include <ofLog.h>
#include <iostream>

ofVec3f createSignedNoiseVec3f(const ofVec3f& position);
ofVec3f createRandomVec3f(const ofVec3f& mins,
                          const ofVec3f& maxs);
ofVec3f createRandomVec3f(float max);
ofVec3f wrapVec(ofVec3f vec, float min, float max);

template<typename T>
T getInterpolated(const T& a, const T& b, float amount);

class Outputable {
public:
  virtual ~Outputable() {}
  virtual void output(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Outputable& obj);

//class HostedLog {
//public:
//  HostedLog(std::string mod, ofLogLevel lvl) {
//    _module = mod;
//    level = lvl;
//  }
//private:
//  std::string _module;
//  friend class LogHost;
//};
//
//class LogHost {
//public:
//  void setLogLevel(ofLogLevel level) {
//    ofSetLogLevel(getLogModule(), level);
//  }
//protected:
//  LogHost(std::string module)
//  : _logNotice(module, OF_LOG_NOTICE)
//  , _logVerbose(module, OF_LOG_VERBOSE)
//  , _logWarning(module, OF_LOG_WARNING) { }
//
//  ofLog& logNotice() { return _logNotice; }
//  ofLog& logVerbose() { return _logVerbose; }
//  ofLog& logWarning() { return _logWarning; }
//  const std::string& getLogModule() const {
//    return _logNotice.module;
//  }
//private:
//
//  HostedLog _logNotice;
//  HostedLog _logVerbose;
//  HostedLog _logWarning;
//};

#endif /* defined(__behavior__Common__) */
