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
  ColorTheme(std::string name);
  ColorTheme() : ColorTheme("Color Theme") { }

  Json to_json() const override;
  void read_json(const Json& obj) override;

  ofParameter<ofFloatColor> background;
  ofParameter<ofFloatColor> bounds;
  ofParameter<ofFloatColor> observerMarker;
  ofParameter<ofFloatColor> occurrenceMarker;
  ofParameter<ofFloatColor> occurrenceRange;
  ofParameter<ofFloatColor> occurrenceConnector;
  ofParameter<ofFloatColor> thresholdConnector;
  ofParameter<ofFloatColor> observerDied;
  ofParameter<ofFloatColor> occurrenceDied;
  ofParameter<ofFloatColor> occurrenceSpawnFailed;
};

#endif /* Colors_h */
