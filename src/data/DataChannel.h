//
//  DataChannel.h
//  memory
//
//  Created by tekt on 10/16/16.
//
//

#ifndef DataChannel_h
#define DataChannel_h

#include <cstring>
#include <string>
#include <ofColor.h>
#include <ofConstants.h>
#include <ofVec3f.h>
#include "../core/Common.h"

class AbstractDataOutputChannel
: public NonCopyable {
public:
  AbstractDataOutputChannel(std::string name)
  : _name(name)
  , _size(0) { }

  const std::string& name() const { return _name; }
  std::size_t size() const { return _size; }

  virtual void unbind() = 0;
  virtual void wipe() = 0;
  virtual bool ready() const = 0;
protected:
  const std::string _name;
  std::size_t _size;
};

template<typename T>
class TypedDataOutputChannelBase
: public AbstractDataOutputChannel {
public:
  TypedDataOutputChannelBase(std::string name)
  : AbstractDataOutputChannel(name) { }

  virtual void setSlice(std::size_t index,
                        const T& value) = 0;
  virtual void setRange(std::size_t start,
                        std::size_t length,
                        const T& value) {
    auto end = start + length;
    for (auto i = start; i < end; ++i) {
      setSlice(i, value);
    }
  }
};

namespace internal {

  template<typename T>
  float convertDataToFloat(const T& value) {
    return static_cast<float>(value);
  }

}

template<typename T>
class ScalarDataOutputChannel
: public TypedDataOutputChannelBase<T> {
public:
  ScalarDataOutputChannel(std::string name)
  : TypedDataOutputChannelBase<T>(name)
  , _data(nullptr) { }

  void bind(float* data, std::size_t size) {
    _data = data;
    AbstractDataOutputChannel::_size = size;
  }

  void unbind() override {
    bind(nullptr,
         0);
  }

  bool ready() const override {
    return _data != nullptr && this->size() > 0;
  }

  void wipe() override {
    //  if (!ready()) {
    //    return;
    //  }
    std::memset(_data, 0, this->size() * sizeof(float));
  }

  void setSlice(std::size_t index, const T& value) override {
    _data[index] = internal::convertDataToFloat(value);
  }

private:
  float* _data;
};

using FloatDataOutputChannel = ScalarDataOutputChannel<float>;
using IntDataOutputChannel = ScalarDataOutputChannel<int>;
using BoolDataOutputChannel = ScalarDataOutputChannel<bool>;

class Vec3fDataOutputChannel
: public TypedDataOutputChannelBase<ofVec3f> {
public:
  Vec3fDataOutputChannel(std::string name);

  void bind(float* xData,
            float* yData,
            float* zData,
            std::size_t size);

  void unbind() override;

  bool ready() const override;

  void wipe() override;

  void setSlice(std::size_t index,
                const ofVec3f& value) override;
private:
  FloatDataOutputChannel _xChannel;
  FloatDataOutputChannel _yChannel;
  FloatDataOutputChannel _zChannel;
};

class ColorDataOutputChannel
: public TypedDataOutputChannelBase<ofFloatColor> {
public:
  ColorDataOutputChannel(std::string name);

  void bind(float* rData,
            float* gData,
            float* bData,
            float* aData,
            std::size_t size);

  void unbind() override;

  bool ready() const override;

  void wipe() override;

  void setSlice(std::size_t index,
                const ofFloatColor& value) override;
private:
  FloatDataOutputChannel _rChannel;
  FloatDataOutputChannel _gChannel;
  FloatDataOutputChannel _bChannel;
  FloatDataOutputChannel _aChannel;
};

#endif /* DataChannel_h */
