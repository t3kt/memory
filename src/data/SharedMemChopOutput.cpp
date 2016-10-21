//
//  SharedMemChopOutput.cpp
//  memory
//
//  Created by tekt on 10/17/16.
//
//

#include "../data/DataChannel.h"
#include "../data/SharedMemChopOutput.h"
#include "../lib/touch/CHOP_SharedMemHeader.h"
#include "../lib/touch/UT_SharedMem.h"

static std::size_t
calculateTotalSize(const DataChannelList& channels,
                   std::size_t maxEntities) {
  std::size_t size = sizeof(CHOP_SharedMemHeader);
  for (const auto& channel : channels) {
    size += channel->name().size() + 1;
  }
  size += channels.size() * maxEntities * sizeof(float);
  return size;
}

SharedMemChopOutput::SharedMemChopOutput(std::string memoryName,
                                         std::size_t maxEntities,
                                         DataChannelList& dataChannels)
: _memoryName(memoryName)
, _maxEntities(maxEntities)
, _dataChannels(dataChannels) { }

bool SharedMemChopOutput::setup() {
  auto size = calculateTotalSize(_dataChannels,
                                 _maxEntities);
  auto channelSize = _maxEntities * sizeof(float);

  _sharedMem.reset(new UT_SharedMem(_memoryName.c_str(), size));
  auto error = _sharedMem->getErrorState();
  if (error != UT_SHM_ERR_NONE) {
    //.. logging and/or exception?
    _sharedMem.reset();
    return false;
  }

  _header =
    reinterpret_cast<CHOP_SharedMemHeader*>(_sharedMem->getMemory());
  _header->magicNumber = CHOP_SHM_MAGIC_NUMBER;
  _header->version = CHOP_SHM_VERSION;
  _header->size = size;
  _header->numChans = _dataChannels.size();
  _header->length = _maxEntities;
  _header->sampleRate = 60; //???
  _header->namesSent = TRUE;
  _header->channelDataOffset = sizeof(CHOP_SharedMemHeader);
  _header->nameDataOffset =
    _header->channelDataOffset + (_header->numChans * channelSize);

  std::size_t offset = 0;
  float* dataPointer = _header->getChannelData();
  for (auto& channel : _dataChannels) {
    channel->bind(dataPointer, _maxEntities);
    dataPointer += channel->dataSize();
  }
  return true;
}

void SharedMemChopOutput::lock() {
  _sharedMem->lock();
}

void SharedMemChopOutput::unlock() {
  _sharedMem->unlock();
}
