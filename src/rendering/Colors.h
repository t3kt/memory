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
#include "../control/Params.h"

class ColorTheme
: public Params {
public:
  ColorTheme() {
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
    add(observerConnector
        .setKey("observerConnector")
        .setName("Observer Connector")
        .setValueAndDefault(ofFloatColor(0.2, 0.5, 0.7, 1)));
    add(occurrenceMarker
        .setKey("occurrenceMarker")
        .setName("Occurrence Marker")
        .setValueAndDefault(ofFloatColor(1, 0.5, 0.34, 1)));
    add(occurrenceRange
        .setKey("occurrenceRange")
        .setName("Occurrence Range")
        .setValueAndDefault(ofFloatColor(0.5, 0.5, 0.5, 0.58)));
    add(occurrenceObserverConnector
        .setKey("occurrenceObserverConnector")
        .setName("Occurrence Observer Connector")
        .setValueAndDefault(ofFloatColor(0.5, 0.5, 0.5, 1)));
    add(occurrenceConnector
        .setKey("occurrenceConnector")
        .setName("Occurrence Connector")
        .setValueAndDefault(ofFloatColor(1, 0.5, 0.34, 1)));
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
    add(fog
        .setKey("fog")
        .setName("Fog")
        .setValueAndDefault(ofFloatColor(0.6, 0.6, 0.6, 0.5)));
  }

  TParam<ofFloatColor> background;
  TParam<ofFloatColor> bounds;
  TParam<ofFloatColor> observerMarker;
  TParam<ofFloatColor> observerThresholdConnector;
  TParam<ofFloatColor> observerConnector;
  TParam<ofFloatColor> occurrenceMarker;
  TParam<ofFloatColor> occurrenceRange;
  TParam<ofFloatColor> occurrenceObserverConnector;
  TParam<ofFloatColor> occurrenceConnector;
  TParam<ofFloatColor> observerDied;
  TParam<ofFloatColor> occurrenceDied;
  TParam<ofFloatColor> occurrenceSpawnFailed;
  TParam<ofFloatColor> fog;
};

#endif /* Colors_h */
