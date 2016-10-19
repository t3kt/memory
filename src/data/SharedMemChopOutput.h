//
//  SharedMemChopOutput.h
//  memory
//
//  Created by tekt on 10/17/16.
//
//

#ifndef SharedMemChopOutput_h
#define SharedMemChopOutput_h

#include <stdint.h>
#include <cstdlib>

class CHOP_SharedMemHeader;
class EntityDataChannels;

class SharedMemChopOutput {
public:
  void bindChannels(EntityDataChannels* channels,
                    std::size_t size);
private:
  CHOP_SharedMemHeader* _chop;
};

#endif /* SharedMemChopOutput_h */
