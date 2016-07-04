//
//  JsonIO.cpp
//  memory
//
//  Created by tekt on 6/30/16.
//
//

#include "JsonIO.h"

#include "AppParameters.h"
#include "AnimationObject.h"
#include "AnimationsController.h"
#include "Bounds.h"
#include "Clock.h"
#include "Colors.h"
#include "Interval.h"
#include "ObserverEntity.h"
#include "ObserversController.h"
#include "OccurrenceEntity.h"
#include "OccurrencesController.h"
#include "ParticleObject.h"
#include "Renderer.h"
#include "ThresholdRenderer.h"
#include "ValueSupplier.h"

#include <ofTypes.h>
#include <ofUtils.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include <functional>

using json11::JsonParse;

static void assertHasShape(const Json& value, Json::shape shape) {
  std::string message;
  if (!value.has_shape(shape, message)) {
    throw JsonException(message);
  }
}

static void assertHasType(const Json& value, Json::Type type) {
  if (value.type() != type) {
    throw JsonException("Incorrect json type: " + value.dump());
  }
}

static void assertHasLength(const Json& value, int length) {
  if (value.array_items().size() != length) {
    throw JsonException("Incorrect length (should be " + ofToString(length) + "): " + value.dump());
  }
}

static Json toJsonValue(const ofVec3f& value) {
  return Json::array { value.x, value.y, value.z };
}

static Json toJsonValue(const ofFloatColor& value) {
  return Json::array { value.r, value.g, value.b, value.a };
}

static ofVec3f ofVec3fFromJsonValue(const Json& value) {
  assertHasType(value, Json::ARRAY);
  assertHasLength(value, 3);
  const Json& x = value[0];
  const Json& y = value[1];
  const Json& z = value[2];
  assertHasType(x, Json::NUMBER);
  assertHasType(y, Json::NUMBER);
  assertHasType(z, Json::NUMBER);
  return ofVec3f(x.number_value(),
                 y.number_value(),
                 z.number_value());
}

static ofFloatColor ofFloatColorFromJsonValue(const Json& value) {
  assertHasType(value, Json::ARRAY);
  assertHasLength(value, 4);
  const Json& r = value[0];
  const Json& g = value[1];
  const Json& b = value[2];
  const Json& a = value[3];
  assertHasType(r, Json::NUMBER);
  assertHasType(g, Json::NUMBER);
  assertHasType(b, Json::NUMBER);
  assertHasType(a, Json::NUMBER);
  return ofFloatColor(r.number_value(),
                      g.number_value(),
                      b.number_value(),
                      a.number_value());
}

static Json merge(const Json obj1, const Json obj2) {
  Json::object out(obj1.object_items());
  out.insert(obj2.object_items().begin(), obj2.object_items().end());
  return out;
}

template<>
Json::Type TParam<ofVec3f>::jsonType = Json::ARRAY;

template<>
Json::Type TParam<ofFloatColor>::jsonType = Json::ARRAY;

template<>
Json::Type TParam<float>::jsonType = Json::NUMBER;

template<>
Json::Type TParam<bool>::jsonType = Json::BOOL;

template<>
Json::Type TParam<int>::jsonType = Json::NUMBER;

template<>
Json TParam<ofVec3f>::to_json() const {
  return toJsonValue(get());
}

template<>
Json TParam<ofFloatColor>::to_json() const {
  return toJsonValue(get());
}

template<>
Json TParam<float>::to_json() const {
  return get();
}

template<>
Json TParam<bool>::to_json() const {
  return get();
}

template<>
Json TParam<int>::to_json() const {
  return get();
}

template<>
void TParam<float>::read_json(const Json& val) {
  assertHasType(val, Json::NUMBER);
  set(val.number_value());
}

template<>
void TParam<int>::read_json(const Json& val) {
  assertHasType(val, Json::NUMBER);
  set(val.number_value());
}

template<>
void TParam<bool>::read_json(const Json& val) {
  assertHasType(val, Json::BOOL);
  set(val.bool_value());
}

template<>
void TParam<ofVec3f>::read_json(const Json& val) {
  set(ofVec3fFromJsonValue(val));
}

template<>
void TParam<ofFloatColor>::read_json(const Json& val) {
  set(ofFloatColorFromJsonValue(val));
}

void readJsonIntoParams(const Json& obj, std::initializer_list<std::reference_wrapper<TParamInfoBase>> params) {
  for (auto iter = params.begin();
       iter != params.end();
       iter++) {
    iter->get().readJsonField(obj);
  }
}

Json paramsToObject(std::initializer_list<std::reference_wrapper<const TParamInfoBase>> params) {
  Json::object obj;
  for (auto iter = params.begin();
       iter != params.end();
       iter++) {
    obj.insert(iter->get().toJsonField());
  }
  return obj;
}

void Params::readJsonField(const Json& obj) {
  const Json& val = obj[getKey()];
  if (!val.is_null()) {
    assertHasType(val, Json::OBJECT);
    read_json(val);
  } else if (hasDefaults()) {
    resetToDefaults();
  } else {
    throw JsonException("Required field missing: " + getKey());
  }
}

template<typename T>
Json ValueRange<T>::to_json() const {
  return paramsToObject({
    lowValue,
    highValue,
  });
}

template<typename T>
void ValueRange<T>::read_json(const Json& val) {
  assertHasType(val, Json::OBJECT);
  readJsonIntoParams(val, {
    lowValue,
    highValue,
  });
}

Json DebugParams::to_json() const {
  return paramsToObject({
    showLog,
    showBounds,
    showStatus,
  });
}

void DebugParams::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    showLog,
    showBounds,
    showStatus,
  });
}

Json CameraParams::to_json() const {
  return paramsToObject({
    spinEnabled,
    spinRate,
  });
}

void CameraParams::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    spinEnabled,
    spinRate,
  });
}

Json MemoryAppParameters::to_json() const {
  return paramsToObject({
    clock,
    debug,
    colors,
    bounds,
    animations,
    observers,
    occurrences,
  });
}

void MemoryAppParameters::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    clock,
    debug,
    colors,
    bounds,
    animations,
    observers,
    occurrences,
  });
}

Json AnimationObject::Params::to_json() const {
  return paramsToObject({
    enabled,
    delay,
    duration,
  });
}

void AnimationObject::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    enabled,
    delay,
    duration,
  });
}

Json ExpandingSphereAnimation::Params::to_json() const {
  return merge(AnimationObject::Params::to_json(),
               paramsToObject({
                 radius,
                 alpha,
               }));
}

void ExpandingSphereAnimation::Params::read_json(const Json& obj) {
  AnimationObject::Params::read_json(obj);
  readJsonIntoParams(obj, {
    radius,
    alpha,
  });
}

Json AnimationsController::Params::to_json() const {
  return paramsToObject({
    enabled,
    observerDied,
    occurrenceDied,
    occurrenceSpawnFailed,
  });
}

void AnimationsController::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    enabled,
    observerDied,
    occurrenceDied,
    occurrenceSpawnFailed,
  });
}

Json SimpleCubeBounds::to_json() const {
  return paramsToObject({
    size,
  });
}

void SimpleCubeBounds::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    size,
  });
}

Json Clock::Params::to_json() const {
  return paramsToObject({
    paused,
    rate,
  });
}

void Clock::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    paused,
    rate,
  });
}

Json ColorTheme::to_json() const {
  return paramsToObject({
    background,
    bounds,
    observerMarker,
    observerThresholdConnector,
    occurrenceMarker,
    occurrenceRange,
    occurrenceConnector,
    observerDied,
    occurrenceDied,
    occurrenceSpawnFailed,
  });
}

void ColorTheme::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    background,
    bounds,
    observerMarker,
    observerThresholdConnector,
    occurrenceMarker,
    occurrenceRange,
    occurrenceConnector,
    observerDied,
    occurrenceDied,
    occurrenceSpawnFailed,
  });
}

Json Interval::Params::to_json() const {
  return paramsToObject({
    interval,
  });
}

void Interval::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    interval,
  });
}

Json ObserverEntity::Params::to_json() const {
  return merge(ParticleObject::Params::to_json(),
               paramsToObject({
                 lifetime,
               }));
}

void ObserverEntity::Params::read_json(const Json& obj) {
  ParticleObject::Params::read_json(obj);
  readJsonIntoParams(obj, {
    lifetime,
  });
}

Json ObserversController::Params::to_json() const {
  return paramsToObject({
    entities,
    spawnInterval,
    initialVelocity,
    occurrenceAttraction,
    spatialNoiseForce,
    renderer,
    threshold,
  });
}

void ObserversController::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    entities,
    spawnInterval,
    initialVelocity,
    occurrenceAttraction,
    spatialNoiseForce,
    renderer,
    threshold,
  });
}

Json OccurrenceEntity::Params::to_json() const {
  return merge(ParticleObject::Params::to_json(),
               paramsToObject({
                 radius,
                 rangeFadeIn,
                 markerSize,
               }));
}

void OccurrenceEntity::Params::read_json(const Json& obj) {
  ParticleObject::Params::read_json(obj);
  readJsonIntoParams(obj, {
    radius,
    rangeFadeIn,
    markerSize,
  });
}

Json OccurrencesController::Params::to_json() const {
  return paramsToObject({
    entities,
    spawnInterval,
    initialVelocity,
    observerAttraction,
    spatialNoiseForce,
    renderer,
    connectorRenderer,
  });
}

void OccurrencesController::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    entities,
    spawnInterval,
    initialVelocity,
    observerAttraction,
    spatialNoiseForce,
    renderer,
    connectorRenderer,
  });
}

Json ParticleObject::Params::to_json() const {
  return paramsToObject({
    damping,
    speed,
  });
}

void ParticleObject::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    damping,
    speed,
  });
}

Json AbstractEntityAttraction::Params::to_json() const {
  return paramsToObject({
    enabled,
    distanceBounds,
    forceRange,
  });
}

void AbstractEntityAttraction::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    enabled,
    distanceBounds,
    forceRange,
  });
}

Json AbstractSpatialNoiseForce::Params::to_json() const {
  return paramsToObject({
    enabled,
    scale,
    rate,
    magnitude,
  });
}

void AbstractSpatialNoiseForce::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    enabled,
    scale,
    rate,
    magnitude,
  });
}

Json AbstractThresholdRenderer::Params::to_json() const {
  return paramsToObject({
    enabled,
    range,
  });
}

void AbstractThresholdRenderer::Params::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    enabled,
    range,
  });
}

Json AbstractEntityRenderer::Params::to_json() const {
  return paramsToObject({
    size,
  });
}

void AbstractEntityRenderer::Params::read_json(const Json &obj) {
  readJsonIntoParams(obj, {
    size,
  });
}

void OccurrenceRenderer::Params::read_json(const Json &obj) {
  AbstractEntityRenderer::Params::read_json(obj);
  readJsonIntoParams(obj, {
    showRange,
    connectionCountRange,
  });
}

Json OccurrenceRenderer::Params::to_json() const {
  return merge(AbstractEntityRenderer::Params::to_json(),
               paramsToObject({
    showRange,
    connectionCountRange,
  }));
}

void ObserverOccurrenceConnectorRenderer::Params::read_json(const json11::Json &obj) {
  readJsonIntoParams(obj, {
    enabled,
    connectionCountRange,
  });
}

Json ObserverOccurrenceConnectorRenderer::Params::to_json() const {
  return paramsToObject({
    enabled,
    connectionCountRange,
  });
}

Json RandomHsbFloatColorSupplier::to_json() const {
  return paramsToObject({
    hueRange,
    saturationRange,
    brightnessRange,
    alphaRange,
  });
}

void RandomHsbFloatColorSupplier::read_json(const Json& obj) {
  readJsonIntoParams(obj, {
    hueRange,
    saturationRange,
    brightnessRange,
    alphaRange,
  });
}

void MemoryAppParameters::readFromFile(std::string filepath) {
  filepath = ofToDataPath(filepath);
  std::ifstream in(filepath.c_str());
  std::stringstream buf;
  buf << in.rdbuf();
  in.close();
  std::string jsonstr = buf.str();
  std::string error;
  Json obj = Json::parse(jsonstr, error, JsonParse::STANDARD);
  if (!error.empty()) {
    throw JsonException(error);
  }
  read_json(obj);
}

void MemoryAppParameters::writeToFile(std::string filepath) const {
  Json obj = to_json();
  filepath = ofToDataPath(filepath);
  std::ofstream out(filepath.c_str());
  out << obj.dump();
  out.close();
}

