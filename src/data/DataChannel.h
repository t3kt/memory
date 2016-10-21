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
#include <memory>
#include <ofColor.h>
#include <ofConstants.h>
#include <ofVec3f.h>
#include <string>
#include <vector>
#include "../core/Common.h"

class AbstractDataOutputChannel
: public NonCopyable {
public:
  AbstractDataOutputChannel(std::string name,
                            std::size_t valueSize)
  : _name(name)
  , _valueSize(valueSize)
  , _length(0) { }

  const std::string& name() const { return _name; }
  std::size_t length() const { return _length; }
  std::size_t dataSize() const {
    return _length * _valueSize;
  }

  virtual void bind(float* data,
                    std::size_t length) = 0;
  virtual void unbind() = 0;
  virtual void wipe() = 0;
  virtual bool ready() const = 0;
protected:
  const std::string _name;
  const std::size_t _valueSize;
  std::size_t _length;
};

template<typename T>
class TypedDataOutputChannelBase
: public AbstractDataOutputChannel {
public:
  TypedDataOutputChannelBase(std::string name,
                             std::size_t valueSize = sizeof(T))
  : AbstractDataOutputChannel(name, valueSize) { }

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

  void bind(float* data, std::size_t length) override {
    _data = data;
    AbstractDataOutputChannel::_length = length;
  }

  void unbind() override {
    _data = nullptr;
    AbstractDataOutputChannel::_length = 0;
  }

  bool ready() const override {
    return _data != nullptr && this->length() > 0;
  }

  void wipe() override {
    //  if (!ready()) {
    //    return;
    //  }
    std::memset(_data, 0, this->dataSize());
  }

  void setSlice(std::size_t index, const T& value) override {
    _data[index] = internal::convertDataToFloat(value);
  }

private:
  float* _data;
};

class Vec3fDataOutputChannel
: public TypedDataOutputChannelBase<ofVec3f> {
public:
  Vec3fDataOutputChannel(std::string name);

  void bind(float* data, std::size_t length) override;

  void unbind() override;

  bool ready() const override;

  void wipe() override;

  void setSlice(std::size_t index,
                const ofVec3f& value) override;
private:
  ScalarDataOutputChannel<float> _xChannel;
  ScalarDataOutputChannel<float> _yChannel;
  ScalarDataOutputChannel<float> _zChannel;
};

class ColorDataOutputChannel
: public TypedDataOutputChannelBase<ofFloatColor> {
public:
  ColorDataOutputChannel(std::string name);

  void bind(float* data, std::size_t length) override;

  void unbind() override;

  bool ready() const override;

  void wipe() override;

  void setSlice(std::size_t index,
                const ofFloatColor& value) override;
private:
  ScalarDataOutputChannel<float> _rChannel;
  ScalarDataOutputChannel<float> _gChannel;
  ScalarDataOutputChannel<float> _bChannel;
  ScalarDataOutputChannel<float> _aChannel;
};

template<typename T>
struct DataChannelT {
  using t = ScalarDataOutputChannel<T>;
};

template<>
struct DataChannelT<ofVec3f> {
  using t = Vec3fDataOutputChannel;
};

template<>
struct DataChannelT<ofFloatColor> {
  using t = ColorDataOutputChannel;
};

template<typename T>
using DataChannelPtr = std::shared_ptr<typename DataChannelT<T>::t>;

using AbstractDataChannelPtr =
std::shared_ptr<AbstractDataOutputChannel>;

class DataChannelList {
public:
  using Storage = std::vector<AbstractDataChannelPtr>;
  using iterator = Storage::iterator;
  using const_iterator = Storage::const_iterator;

  iterator begin() { return _channels.begin(); }
  iterator end() { return _channels.end(); }
  const_iterator begin() const { return _channels.begin(); }
  const_iterator end() const { return _channels.end(); }
  std::size_t size() const { return _channels.size(); }

  template<typename T>
  DataChannelPtr<T> add(std::string name) {
    auto channel = std::make_shared<typename DataChannelT<T>::t>(name);
    _channels.push_back(channel);
    return channel;
  }

private:
  Storage _channels;
};

#endif /* DataChannel_h */
