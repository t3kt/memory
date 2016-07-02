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
#include "ThresholdRenderer.h"
#include "ValueSupplier.h"

#include <ofTypes.h>
#include <ofUtils.h>
#include <iostream>
#include <fstream>
#include <sstream>

using json11::JsonParse;

class JsonException {
public:
  JsonException(std::string msg) : message(msg) { }
  std::string message;
};

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

Json DebugParams::to_json() const {
  return Json::object {
    {"showLog", showLog.get()},
    {"showBounds", showBounds.get()},
    {"showStatus", showStatus.get()},
  };
}

void DebugParams::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"showLog", Json::BOOL},
    {"showBounds", Json::BOOL},
    {"showStatus", Json::BOOL},
  });
  showLog.set(obj["showLog"].bool_value());
  showBounds.set(obj["showBounds"].bool_value());
  showStatus.set(obj["showStatus"].bool_value());
}

Json CameraParams::to_json() const {
  return Json::object {
    {"spinEnabled", spinEnabled.get()},
    {"spinRate", toJsonValue(spinRate.get())},
  };
}

void CameraParams::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"spinEnabled", Json::BOOL},
    {"spinRate", Json::ARRAY},
  });
  spinEnabled.set(obj["spinEnabled"].bool_value());
  spinRate.set(ofVec3fFromJsonValue(obj["spinRate"]));
}

Json MemoryAppParameters::to_json() const {
  return Json::object {
    {"clock", clock},
    {"debug", debug},
    {"colors", colors},
    {"bounds", bounds},
    {"animations", animations},
    {"observers", observers},
    {"occurrences", occurrences},
  };
}

void MemoryAppParameters::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"clock", Json::OBJECT},
    {"debug", Json::OBJECT},
    {"colors", Json::OBJECT},
    {"bounds", Json::OBJECT},
    {"animations", Json::OBJECT},
    {"observers", Json::OBJECT},
    {"occurrences", Json::OBJECT},

  });
}

Json AnimationObject::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"delay", delay.get()},
    {"duration", duration.get()},
  };
}

void AnimationObject::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"enabled", Json::BOOL},
    {"delay", Json::NUMBER},
    {"duration", Json::NUMBER},
  });
  enabled.set(obj["enabled"].bool_value());
  delay.set(obj["delay"].number_value());
  duration.set(obj["duration"].number_value());
}

Json ExpandingSphereAnimation::Params::to_json() const {
  return merge(AnimationObject::Params::to_json(),
               Json::object {
                 {"radius", radius},
                 {"alpha", alpha},
               });
}

void ExpandingSphereAnimation::Params::read_json(const Json& obj) {
  AnimationObject::Params::read_json(obj);
  assertHasShape(obj, Json::shape {
    {"radius", Json::ARRAY},
    {"alpha", Json::ARRAY},
  });
  radius.read_json(obj["radius"]);
  alpha.read_json(obj["alpha"]);
}

Json AnimationsController::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"observerDied", observerDied},
    {"occurrenceDied", occurrenceDied},
    {"occurrenceSpawnFailed", occurrenceSpawnFailed},
  };
}

void AnimationsController::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"enabled", Json::BOOL},
    {"observerDied", Json::OBJECT},
    {"occurrenceDied", Json::OBJECT},
    {"occurrenceSpawnFailed", Json::OBJECT},
  });
  enabled.set(obj["enabled"].bool_value());
  observerDied.read_json(obj["observerDied"]);
  occurrenceDied.read_json(obj["occurrenceDied"]);
  occurrenceSpawnFailed.read_json(obj["occurrenceSpawnFailed"]);
}

Json SimpleCubeBounds::to_json() const {
  return Json::object {
    {"size", size.get()},
  };
}

void SimpleCubeBounds::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"size", Json::NUMBER},
  });
  size.set(obj["size"].number_value());
}

Json Clock::Params::to_json() const {
  return Json::object {
    {"paused", paused.get()},
    {"rate", rate.get()},
  };
}

void Clock::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"paused", Json::BOOL},
    {"rate", Json::NUMBER},
  });
  paused.set(obj["paused"].bool_value());
  rate.set(obj["rate"].number_value());
}

Json ColorTheme::to_json() const {
  return Json::object {
    {"background", toJsonValue(background.get())},
    {"bounds", toJsonValue(bounds.get())},
    {"observerMarker", toJsonValue(observerMarker.get())},
    {"occurrenceMarker", toJsonValue(occurrenceMarker.get())},
    {"occurrenceRange", toJsonValue(occurrenceRange.get())},
    {"occurrenceConnector", toJsonValue(occurrenceConnector.get())},
    {"thresholdConnector", toJsonValue(thresholdConnector.get())},
    {"observerDied", toJsonValue(observerDied.get())},
    {"occurrenceDied", toJsonValue(occurrenceDied.get())},
    {"occurrenceSpawnFailed", toJsonValue(occurrenceSpawnFailed.get())},
  };
}

void ColorTheme::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"background", Json::OBJECT},
    {"bounds", Json::OBJECT},
    {"observerMarker", Json::OBJECT},
    {"occurrenceMarker", Json::OBJECT},
    {"occurrenceRange", Json::OBJECT},
    {"occurrenceConnector", Json::OBJECT},
    {"thresholdConnector", Json::OBJECT},
    {"observerDied", Json::OBJECT},
    {"occurrenceDied", Json::OBJECT},
    {"occurrenceSpawnFailed", Json::OBJECT},
  });
  background.set(ofFloatColorFromJsonValue(obj["background"]));
  bounds.set(ofFloatColorFromJsonValue(obj["bounds"]));
  observerMarker.set(ofFloatColorFromJsonValue(obj["observerMarker"]));
  occurrenceMarker.set(ofFloatColorFromJsonValue(obj["occurrenceMarker"]));
  occurrenceRange.set(ofFloatColorFromJsonValue(obj["occurrenceRange"]));
  occurrenceConnector.set(ofFloatColorFromJsonValue(obj["occurrenceConnector"]));
  thresholdConnector.set(ofFloatColorFromJsonValue(obj["thresholdConnector"]));
  observerDied.set(ofFloatColorFromJsonValue(obj["observerDied"]));
  occurrenceDied.set(ofFloatColorFromJsonValue(obj["occurrenceDied"]));
  occurrenceSpawnFailed.set(ofFloatColorFromJsonValue(obj["occurrenceSpawnFailed"]));
}

Json Interval::Params::to_json() const {
  return Json::object {
    {"interval", interval},
  };
}

void Interval::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"interval", Json::ARRAY},
  });
  interval.read_json(obj["interval"]);
}

Json ObserverEntity::Params::to_json() const {
  return merge(ParticleObject::Params::to_json(),
               Json::object {
                 {"lifetime", lifetime},
                 {"drawRadius", drawRadius.get()},
               });
}

void ObserverEntity::Params::read_json(const Json& obj) {
  ParticleObject::Params::read_json(obj);
  assertHasShape(obj, Json::shape {
    {"lifetime", Json::OBJECT},
    {"drawRadius", Json::NUMBER},
  });
  lifetime.read_json(obj["lifetime"]);
  drawRadius.set(obj["drawRadius"].number_value());
}

Json ObserversController::Params::to_json() const {
  return Json::object {
    {"entities", entities},
    {"spawnInterval", spawnInterval},
    {"initialVelocity", initialVelocity},
    {"occurrenceAttraction", occurrenceAttraction},
    {"spatialNoiseForce", spatialNoiseForce},
    {"threshold", threshold},
  };
}

void ObserversController::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"entities", Json::OBJECT},
    {"spawnInterval", Json::OBJECT},
    {"initialVelocity", Json::OBJECT},
    {"occurrenceAttraction", Json::OBJECT},
    {"spatialNoiseForce", Json::OBJECT},
    {"threshold", Json::OBJECT},
  });
  entities.read_json(obj["entities"]);
  spawnInterval.read_json(obj["spawnInterval"]);
  initialVelocity.read_json(obj["initialVelocity"]);
  occurrenceAttraction.read_json(obj["occurrenceAttraction"]);
  spatialNoiseForce.read_json(obj["spatialNoiseForce"]);
  threshold.read_json(obj["threshold"]);
}

Json OccurrenceEntity::Params::to_json() const {
  return merge(ParticleObject::Params::to_json(),
               Json::object {
                 {"radius", radius},
                 {"rangeFadeIn", rangeFadeIn.get()},
                 {"markerSize", markerSize.get()},
               });
}

void OccurrenceEntity::Params::read_json(const Json& obj) {
  ParticleObject::Params::read_json(obj);
  assertHasShape(obj, Json::shape {
    {"radius", Json::OBJECT},
    {"rangeFadeIn", Json::NUMBER},
    {"markerSize", Json::NUMBER},
  });
}

Json OccurrencesController::Params::to_json() const {
  return Json::object {
    {"entities", entities},
    {"spawnInterval", spawnInterval},
    {"initialVelocity", initialVelocity},
    {"observerAttraction", observerAttraction},
    {"spatialNoiseForce", spatialNoiseForce},
  };
}

void OccurrencesController::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"entities", Json::OBJECT},
    {"spawnInterval", Json::OBJECT},
    {"initialVelocity", Json::OBJECT},
    {"observerAttraction", Json::OBJECT},
    {"spatialNoiseForce", Json::OBJECT},
  });
  entities.read_json(obj["entities"]);
  spawnInterval.read_json(obj["spawnInterval"]);
  initialVelocity.read_json(obj["initialVelocity"]);
  observerAttraction.read_json(obj["observerAttraction"]);
  spatialNoiseForce.read_json(obj["spatialNoiseForce"]);
}

Json ParticleObject::Params::to_json() const {
  return Json::object {
    {"damping", damping.get()},
    {"speed", speed.get()},
  };
}

void ParticleObject::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"damping", Json::NUMBER},
    {"speed", Json::NUMBER},
  });
}

Json AbstractEntityAttraction::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"distanceBounds", distanceBounds},
    {"forceRange", forceRange},
  };
}

void AbstractEntityAttraction::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"enabled", Json::BOOL},
    {"distanceBounds", Json::OBJECT},
    {"forceRange", Json::OBJECT},
  });
  enabled.set(obj["enabled"].bool_value());
  distanceBounds.read_json(obj["distanceBounds"]);
  forceRange.read_json(obj["forceRange"]);
}

Json AbstractSpatialNoiseForce::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"scale", scale.get()},
    {"rate", rate.get()},
    {"magnitude", magnitude.get()},
  };
}

void AbstractSpatialNoiseForce::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"enabled", Json::BOOL},
    {"scale", Json::NUMBER},
    {"rate", Json::NUMBER},
    {"magnitude", Json::NUMBER},
  });
  enabled.set(obj["enabled"].bool_value());
  scale.set(obj["scale"].number_value());
  rate.set(obj["rate"].number_value());
  magnitude.set(obj["magnitude"].number_value());
}

Json AbstractThresholdRenderer::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"range", range},
  };
}

void AbstractThresholdRenderer::Params::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"enabled", Json::BOOL},
    {"range", Json::OBJECT},
  });
  enabled.set(obj["enabled"].bool_value());
  range.read_json(obj["range"]);
}

Json RandomHsbFloatColorSupplier::to_json() const {
  return Json::object {
    {"hueRange", _hueRange},
    {"saturationRange", _saturationRange},
    {"brightnessRange", _brightnessRange},
    {"alphaRange", _alphaRange},
  };
}

void RandomHsbFloatColorSupplier::read_json(const Json& obj) {
  assertHasShape(obj, Json::shape {
    {"hueRange", Json::OBJECT},
    {"saturationRange", Json::OBJECT},
    {"brightnessRange", Json::OBJECT},
    {"alphaRange", Json::OBJECT},
  });
  _hueRange.read_json(obj["hueRange"]);
  _saturationRange.read_json(obj["saturationRange"]);
  _brightnessRange.read_json(obj["brightnessRange"]);
  _alphaRange.read_json(obj["alphaRange"]);
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

