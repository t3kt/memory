//
//  DataChannel.cpp
//  memory
//
//  Created by tekt on 10/16/16.
//
//

#include <cstring>
#include <ofVec3f.h>
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
