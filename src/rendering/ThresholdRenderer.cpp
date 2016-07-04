//
//  ThresholdRenderer.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/27/16.
//
//

#include "ThresholdRenderer.h"
#include <ofMath.h>

AbstractThresholdRenderer::Params::Params()
: ::Params() {
  add(_enabled
      .setKey("enabled")
      .setName("Enabled")
      .setValueAndDefault(true));
//  add(color.set("Color", ofFloatColor(0.7, 0.7, 0.85, 0.4)));
  add(range
      .setKey("range")
      .setName("Range")
      .setParamValuesAndDefaults(0, 0.1)
      .setParamRanges(0, 8));
}

ThreshData::ThreshData() {
  _mesh.setMode(OF_PRIMITIVE_LINES);
}

void ThreshData::initialize(float minDist, float maxDist, ofFloatColor color) {
  _mesh.clear();
  _minDist = minDist;
  _maxDist = maxDist;
  _color = color;
}

bool ThreshData::tryAdd(ofVec3f pos1, ofVec3f pos2) {
  float dist = pos1.distance(pos2);
  if (dist < _minDist || dist > _maxDist) {
    return false;
  }
  ofFloatColor color = _color;
  color.a *= ofMap(dist, _minDist, _maxDist, 1, 0);
  _mesh.addVertex(pos1);
  _mesh.addVertex(pos2);
  _mesh.addColor(color);
  _mesh.addColor(color);

  return true;
}

void AbstractThresholdRenderer::update(const State &state) {
  if (!_params.enabled()) {
    return;
  }
  _data.initialize(_params.range.lowValue(),
                   _params.range.highValue(),
                   _color);
  populateThreshData(&_data);
}

void AbstractThresholdRenderer::draw(const State &state) {
  if (!_params.enabled()) {
    return;
  }
  _data._mesh.draw();
}
