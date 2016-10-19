//
//  DataChannel.cpp
//  memory
//
//  Created by tekt on 10/16/16.
//
//

#include <cstring>
#include <ofConstants.h>
#include <ofVec3f.h>
#include <type_traits>
#include "../data/DataChannel.h"

template<>
class DataOutputChannel<float>
: public AbstractDataOutputChannel {
public:
  DataOutputChannel(std::string name)
  : AbstractDataOutputChannel(name)
  , _data(nullptr) { }

  void bind(float* data, std::size_t size) {
    _data = data;
    _size = size;
  }

  void unbind() {
    bind(nullptr, 0);
  }

  bool ready() const override {
    return _data != nullptr && _size > 0;
  }

  void wipe() override {
    //  if (!ready()) {
    //    return;
    //  }
    std::memset(_data, 0, size() * sizeof(float));
  }

  void setSlice(std::size_t index, const float& value) {
    _data[index] = value;
  }
private:
  float* _data;
};

template<typename T>
class StaticCastDataOutputChannel
: public AbstractDataOutputChannel {
public:
  StaticCastDataOutputChannel(std::string name)
  : AbstractDataOutputChannel(name)
  , _channel(name) { }

  void bind(float* data, std::size_t size) {
    _channel.bind(data, size);
  }

  void unbind() {
    _channel.unbind();
  }

  bool ready() const override {
    return _channel.ready();
  }

  void wipe() override {
    _channel.wipe();
  }

  void setSlice(std::size_t index, const T& value) {
    _channel.setSlice(index, static_cast<float>(value));
  }

private:
  DataOutputChannel<float> _channel;
};

template<>
class DataOutputChannel<int>
: public StaticCastDataOutputChannel<int> {
public:
  DataOutputChannel(std::string name)
  : StaticCastDataOutputChannel(name) { }
};

template<>
class DataOutputChannel<bool>
: public StaticCastDataOutputChannel<bool> {
public:
  DataOutputChannel(std::string name)
  : StaticCastDataOutputChannel(name) { }
};

template<>
class DataOutputChannel<ofVec3f>
: public AbstractDataOutputChannel {
public:
  DataOutputChannel(std::string name,
                    std::string xSuffix = "x",
                    std::string ySuffix = "y",
                    std::string zSuffix = "z")
  : AbstractDataOutputChannel(name)
  , _xChannel(name + xSuffix)
  , _yChannel(name + ySuffix)
  , _zChannel(name + zSuffix) { }

  void bind(float* xData,
            float* yData,
            float* zData,
            std::size_t size) {
    _xChannel.bind(xData, size);
    _yChannel.bind(yData, size);
    _zChannel.bind(zData, size);
    _size = size;
  }

  void unbind() {
    bind(nullptr,
         nullptr,
         nullptr,
         0);
  }

  bool ready() const override {
    return _xChannel.ready() && _yChannel.ready() && _zChannel.ready() && _size > 0;
  }

  void wipe() override {
    _xChannel.wipe();
    _yChannel.wipe();
    _zChannel.wipe();
  }
private:
  DataOutputChannel<float> _xChannel;
  DataOutputChannel<float> _yChannel;
  DataOutputChannel<float> _zChannel;
};

template<>
class DataOutputChannel<ofFloatColor>
: public AbstractDataOutputChannel {
public:
  DataOutputChannel(std::string name,
                    std::string rSuffix = "r",
                    std::string gSuffix = "g",
                    std::string bSuffix = "b",
                    std::string aSuffix = "a")
  : AbstractDataOutputChannel(name)
  , _rChannel(name + rSuffix)
  , _gChannel(name + gSuffix)
  , _bChannel(name + bSuffix)
  , _aChannel(name + aSuffix) { }

  void bind(float* rData,
            float* gData,
            float* bData,
            float* aData,
            std::size_t size) {
    _rChannel.bind(rData, size);
    _gChannel.bind(gData, size);
    _bChannel.bind(bData, size);
    _aChannel.bind(aData, size);
    _size = size;
  }

  void unbind() {
    bind(nullptr,
         nullptr,
         nullptr,
         nullptr,
         0);
  }

  bool ready() const override {
    return _rChannel.ready() && _gChannel.ready() && _bChannel.ready() && _aChannel.ready() && _size > 0;
  }

  void wipe() override {
    _rChannel.wipe();
    _gChannel.wipe();
    _bChannel.wipe();
    _aChannel.wipe();
  }
private:
  DataOutputChannel<float> _rChannel;
  DataOutputChannel<float> _gChannel;
  DataOutputChannel<float> _bChannel;
  DataOutputChannel<float> _aChannel;
};
