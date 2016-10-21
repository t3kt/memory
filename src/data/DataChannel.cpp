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
#include "../data/DataChannel.h"

Vec3fDataOutputChannel::Vec3fDataOutputChannel(std::string name)
: TypedDataOutputChannelBase(name)
, _xChannel(name + 'x')
, _yChannel(name + 'y')
, _zChannel(name + 'z') { }

void Vec3fDataOutputChannel::bind(float *data, std::size_t length) {
  _xChannel.bind(data, length);
  data += _xChannel.dataSize();
  _yChannel.bind(data, length);
  data += _yChannel.dataSize();
  _zChannel.bind(data, length);
  _length = length;
}

void Vec3fDataOutputChannel::unbind() {
  _xChannel.unbind();
  _yChannel.unbind();
  _zChannel.unbind();
  _length = 0;
}

bool Vec3fDataOutputChannel::ready() const {
  return _xChannel.ready() && _yChannel.ready() && _zChannel.ready() && _length > 0;
}

void Vec3fDataOutputChannel::wipe() {
  _xChannel.wipe();
  _yChannel.wipe();
  _zChannel.wipe();
}

void Vec3fDataOutputChannel::setSlice(std::size_t index,
                                      const ofVec3f &value) {
  _xChannel.setSlice(index, value.x);
  _yChannel.setSlice(index, value.y);
  _zChannel.setSlice(index, value.z);
}

ColorDataOutputChannel::ColorDataOutputChannel(std::string name)
: TypedDataOutputChannelBase(name)
, _rChannel(name + 'r')
, _gChannel(name + 'g')
, _bChannel(name + 'b')
, _aChannel(name + 'a') { }

void ColorDataOutputChannel::bind(float* data,
                                  std::size_t length) {
  _rChannel.bind(data, length);
  data += _rChannel.dataSize();
  _gChannel.bind(data, length);
  data += _gChannel.dataSize();
  _bChannel.bind(data, length);
  data += _bChannel.dataSize();
  _aChannel.bind(data, length);
  _length = length;
}

void ColorDataOutputChannel::unbind() {
  _rChannel.unbind();
  _gChannel.unbind();
  _bChannel.unbind();
  _aChannel.unbind();
  _length = 0;
}

bool ColorDataOutputChannel::ready() const {
  return _rChannel.ready() && _gChannel.ready() && _bChannel.ready() && _aChannel.ready() && _length > 0;
}

void ColorDataOutputChannel::wipe() {
  _rChannel.wipe();
  _gChannel.wipe();
  _bChannel.wipe();
  _aChannel.wipe();
}

void ColorDataOutputChannel::setSlice(std::size_t index,
                                      const ofFloatColor& value) {
  _rChannel.setSlice(index, value.r);
  _gChannel.setSlice(index, value.g);
  _bChannel.setSlice(index, value.b);
  _aChannel.setSlice(index, value.a);
}
