//
//  Colors.cpp
//  memory
//
//  Created by tekt on 6/29/16.
//
//

#include "Colors.h"

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
  add(thresholdConnector
      .setKey("thresholdConnector")
      .setName("Threshold Connector")
      .setValueAndDefault(ofFloatColor(0.7, 0.7, 0.85, 0.4)));
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
}
