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

void Vec3fDataOutputChannel::bind(float* xData,
                                  float* yData,
                                  float* zData,
                                  std::size_t size) {
  _xChannel.bind(xData, size);
  _yChannel.bind(yData, size);
  _zChannel.bind(zData, size);
  _size = size;
}

void Vec3fDataOutputChannel::unbind() {
  bind(nullptr,
       nullptr,
       nullptr,
       0);
}

bool Vec3fDataOutputChannel::ready() const {
  return _xChannel.ready() && _yChannel.ready() && _zChannel.ready() && _size > 0;
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

void ColorDataOutputChannel::bind(float* rData,
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

void ColorDataOutputChannel::unbind() {
  bind(nullptr,
       nullptr,
       nullptr,
       nullptr,
       0);
}

bool ColorDataOutputChannel::ready() const {
  return _rChannel.ready() && _gChannel.ready() && _bChannel.ready() && _aChannel.ready() && _size > 0;
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
