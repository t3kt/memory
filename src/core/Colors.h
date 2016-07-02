//
//  Colors.h
//  memory
//
//  Created by tekt on 6/29/16.
//
//

#ifndef Colors_h
#define Colors_h

#include <ofColor.h>
#include "Params.h"

class ColorTheme
: public Params {
public:
  ColorTheme();

  Json to_json() const override;
  void read_json(const Json& obj) override;

  TParam<ofFloatColor> background;
  TParam<ofFloatColor> bounds;
  TParam<ofFloatColor> observerMarker;
  TParam<ofFloatColor> occurrenceMarker;
  TParam<ofFloatColor> occurrenceRange;
  TParam<ofFloatColor> occurrenceConnector;
  TParam<ofFloatColor> thresholdConnector;
  TParam<ofFloatColor> observerDied;
  TParam<ofFloatColor> occurrenceDied;
  TParam<ofFloatColor> occurrenceSpawnFailed;
};

#endif /* Colors_h */
