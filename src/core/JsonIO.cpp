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

Json toJsonValue(const ofVec3f& value) {
  return Json::array { value.x, value.y, value.z };
}

Json toJsonValue(const ofFloatColor& value) {
  return Json::array { value.r, value.g, value.b, value.a };
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

