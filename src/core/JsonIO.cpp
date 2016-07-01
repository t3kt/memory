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

#include <ofUtils.h>

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

Json toJsonValue(const ofVec3f& value) {
  return Json::array { value.x, value.y, value.z };
}

Json toJsonValue(const ofFloatColor& value) {
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

Json DebugParams::to_json() const {
  return Json::object {
    {"showLog", showLog.get()},
    {"showBounds", showBounds.get()},
    {"showStatus", showStatus.get()},
  };
}

Json CameraParams::to_json() const {
  return Json::object {
    {"spinEnabled", spinEnabled.get()},
    {"spinRate", toJsonValue(spinRate.get())},
  };
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

Json AnimationObject::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"delay", delay.get()},
    {"duration", duration.get()},
  };
}

Json ExpandingSphereAnimation::Params::to_json() const {
  return merge(AnimationObject::Params::to_json(),
               Json::object {
                 {"radius", radius},
                 {"alpha", alpha},
               });
}

Json AnimationsController::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"observerDied", observerDied},
    {"occurrenceDied", occurrenceDied},
    {"occurrenceSpawnFailed", occurrenceSpawnFailed},
  };
}

Json SimpleCubeBounds::to_json() const {
  return Json::object {
    {"size", size.get()},
  };
}

Json Clock::Params::to_json() const {
  return Json::object {
    {"paused", paused.get()},
    {"rate", rate.get()},
  };
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

Json Interval::Params::to_json() const {
  return Json::object {
    {"interval", interval},
  };
}

Json ObserverEntity::Params::to_json() const {
  return merge(ParticleObject::Params::to_json(),
               Json::object {
                 {"lifetime", lifetime},
                 {"drawRadius", drawRadius.get()},
               });
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

Json OccurrenceEntity::Params::to_json() const {
  return merge(ParticleObject::Params::to_json(),
               Json::object {
                 {"radius", radius},
                 {"rangeFadeIn", rangeFadeIn.get()},
                 {"markerSize", markerSize.get()},
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

Json ParticleObject::Params::to_json() const {
  return Json::object {
    {"damping", damping.get()},
    {"speed", speed.get()},
  };
}

Json AbstractEntityAttraction::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"distanceBounds", distanceBounds},
    {"forceRange", forceRange},
  };
}

Json AbstractSpatialNoiseForce::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"scale", scale.get()},
    {"rate", rate.get()},
    {"magnitude", magnitude.get()},
  };
}

Json AbstractThresholdRenderer::Params::to_json() const {
  return Json::object {
    {"enabled", enabled.get()},
    {"range", range},
  };
}

