//
//  State.h
//  behavior
//
//  Created by tekt on 1/19/15.
//
//

#ifndef __behavior__State__
#define __behavior__State__

#include "Common.h"
#include <ofxLiquidEvent.h>

class State : public Outputable {
public:
  State();
  
  void output(std::ostream& os) const override;
  
  float time;
  float timeDelta;
};

class ChangeFlag : public Outputable {
public:
  ChangeFlag(std::string name)
  : _name(name)
  , _status(false) {}

  const std::string& name() const { return _name; }

  void set() { _status = true; }
  void reset() { _status = false; }

  operator bool() const { return _status; }

  void output(std::ostream& os) const override;

  template<typename T>
  void bindToEvent(ofxLiquidEvent<T>& event) {
    event += [&](T e) {
      set();
    };
  }
private:
  const std::string _name;
  bool _status;
};

class ChangeFlags : public Outputable {
public:
  ChangeFlags();

  void clear();

  void output(std::ostream& os) const override;

  ChangeFlag boundsChanged;
};

#endif /* defined(__behavior__State__) */
