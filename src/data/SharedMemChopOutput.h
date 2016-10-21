//
//  SharedMemChopOutput.h
//
//

#pragma once

#include <cstdlib>
#include <memory>

class CHOP_SharedMemHeader;
class DataChannelList;
class UT_SharedMem;

class SharedMemChopOutput {
public:
  SharedMemChopOutput(std::string memoryName,
                      std::size_t maxEntities,
                      DataChannelList& dataChannels);

  bool setup();
private:
  const std::string _memoryName;
  const std::size_t _maxEntities;
  std::unique_ptr<UT_SharedMem> _sharedMem;
  CHOP_SharedMemHeader* _header;
  DataChannelList& _dataChannels;
};

