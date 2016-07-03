//
//  Colors.h
//  memory
//
//  Created by tekt on 6/29/16.
//
//

#ifndef Colors_h
#define Colors_h

#include <functional>
#include <map>
#include <initializer_list>
#include <ofColor.h>
#include "Common.h"
#include "Params.h"

enum class ColorId {
  BACKGROUND,
  BOUNDS,
  OBSERVER_MARKER,
  OBSERVER_THRESHOLD_CONNECTOR,
  OCCURRENCE_MARKER,
  OCCURRENCE_RANGE,
  OCCURRENCE_CONNECTOR,
  OBSERVER_DIED,
  OCCURRENCE_DIED,
  OCCURRENCE_SPAWN_FAILED,
};

extern EnumTypeInfo<ColorId> ColorIdType;

class ColorTheme
: public Params {
public:
  ColorTheme();

  Json to_json() const override;
  void read_json(const Json& obj) override;

  TParam<ofFloatColor> background;
  TParam<ofFloatColor> bounds;
  TParam<ofFloatColor> observerMarker;
  TParam<ofFloatColor> observerThresholdConnector;
  TParam<ofFloatColor> occurrenceMarker;
  TParam<ofFloatColor> occurrenceRange;
  TParam<ofFloatColor> occurrenceConnector;
  TParam<ofFloatColor> observerDied;
  TParam<ofFloatColor> occurrenceDied;
  TParam<ofFloatColor> occurrenceSpawnFailed;

  const ofFloatColor& getColor(ColorId colorId) const;
private:
  void registerColorFields(std::initializer_list<std::reference_wrapper<TParam<ofFloatColor>>> params);
  std::map<ColorId, std::reference_wrapper<TParam<ofFloatColor>>> _colorParamsById;
};

#endif /* Colors_h */
