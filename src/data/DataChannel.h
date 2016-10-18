//
//  DataChannel.h
//  memory
//
//  Created by tekt on 10/16/16.
//
//

#ifndef DataChannel_h
#define DataChannel_h

#include <string>
#include "../core/Common.h"

class AbstractDataOutputChannel
: public NonCopyable {
public:
  AbstractDataOutputChannel(std::string name)
  : _name(name)
  , _size(0) { }

  const std::string& name() const { return _name; }
  std::size_t size() const { return _size; }

  virtual void wipe() = 0;
  virtual bool ready() const = 0;
protected:
  const std::string _name;
  std::size_t _size;
};

template<typename T>
class DataOutputChannel;

#endif /* DataChannel_h */
