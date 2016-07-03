//
//  Colors.cpp
//  memory
//
//  Created by tekt on 6/29/16.
//
//

#include "Colors.h"

EnumTypeInfo<ColorId> ColorIdType({
  {"background", ColorId::BACKGROUND},
  {"bounds", ColorId::BOUNDS},
  {"observerMarker", ColorId::OBSERVER_MARKER},
  {"observerThresholdConnector", ColorId::OBSERVER_THRESHOLD_CONNECTOR},
  {"occurrenceMarker", ColorId::OCCURRENCE_MARKER},
  {"occurrenceRange", ColorId::OCCURRENCE_RANGE},
  {"occurrenceConnector", ColorId::OCCURRENCE_CONNECTOR},
  {"observerDied", ColorId::OBSERVER_DIED},
  {"occurrenceDied", ColorId::OCCURRENCE_DIED},
  {"occurrenceSpawnFailed", ColorId::OCCURRENCE_SPAWN_FAILED},
});

ColorTheme::ColorTheme()
: ::Params() {
  add(background
      .setKey("background")
      .setName("Background")
      .setValueAndDefault(ofFloatColor(0, 0, 0, 1)));
  add(bounds
      .setKey("bounds")
      .setName("Bounds")
      .setValueAndDefault(ofFloatColor(0.2, 0.2, 0.2, 0.3)));
  add(observerMarker
      .setKey("observerMarker")
      .setName("Observer Marker")
      .setValueAndDefault(ofFloatColor(0.525, 0.7, 0.35, 1)));
  add(observerThresholdConnector
      .setKey("observerThresholdConnector")
      .setName("Observer Threshold Connector")
      .setValueAndDefault(ofFloatColor(0.7, 0.7, 0.85, 0.4)));
  add(occurrenceMarker
      .setKey("occurrenceMarker")
      .setName("Occurrence Marker")
      .setValueAndDefault(ofFloatColor(1, 0.5, 0.34, 1)));
  add(occurrenceRange
      .setKey("occurrenceRange")
      .setName("Occurrence Range")
      .setValueAndDefault(ofFloatColor(0.5, 0.5, 0.5, 0.58)));
  add(occurrenceConnector
      .setKey("occurrenceConnector")
      .setName("Occurrence Connector")
      .setValueAndDefault(ofFloatColor(0.5, 0.5, 0.5, 1)));
  add(observerDied
      .setKey("observerDied")
      .setName("Observer Died")
      .setValueAndDefault(ofFloatColor(0.660006, 0.644068, 0.787503, 0.686568)));
  add(occurrenceDied
      .setKey("occurrenceDied")
      .setName("Occurrence Died")
      .setValueAndDefault(ofFloatColor(0.766254, 0.644068, 0.452822, 0.792816)));
  add(occurrenceSpawnFailed
      .setKey("occurrenceSpawnFailed")
      .setName("Occurrence Spawn Failed")
      .setValueAndDefault(ofFloatColor(0.814066, 0.681255, 0.511258, 0.569695)));
  registerColorFields({
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

void ColorTheme::registerColorFields(std::initializer_list<std::reference_wrapper<TParam<ofFloatColor>>> params) {
  for (std::reference_wrapper<TParam<ofFloatColor>> param : params) {
    ColorId colorId = ColorIdType.parseString(param.get().getKey());
    _colorParamsById.insert(std::make_pair(colorId, param));
  }
}

const ofFloatColor& ColorTheme::getColor(ColorId colorId) const {
  auto iter = _colorParamsById.find(colorId);
  if (iter == _colorParamsById.end()) {
    throw std::invalid_argument("Unknown color: " + ColorIdType.toString(colorId));
  }
  return iter->second.get();
}
