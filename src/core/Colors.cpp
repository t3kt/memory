//
//  Colors.cpp
//  memory
//
//  Created by tekt on 6/29/16.
//
//

#include "Colors.h"

ColorTheme::ColorTheme(std::string name)
: ::Params(name) {
  add(background.set("Background", ofFloatColor::white));
  add(bounds.set("Bounds", ofFloatColor(0.2, 0.2, 0.2, 0.3)));
  add(observerMarker.set("Observer Marker", ofFloatColor(0.525, 0.7, 0.35, 1)));
  add(occurrenceMarker.set("Occurrence Marker", ofFloatColor(1, 0.5, 0.34, 1)));
  add(occurrenceRange.set("Occurrence Range", ofFloatColor(0.5, 0.5, 0.5, 0.58)));
  add(occurrenceConnector.set("Occurrence Connector", ofFloatColor(0.5, 0.5, 0.5, 1)));
  add(thresholdConnector.set("Threshold Connector", ofFloatColor(0.7, 0.7, 0.85, 0.4)));
  add(observerDied.set("Observer Died", ofFloatColor(0.660006, 0.644068, 0.787503, 0.686568)));
  add(occurrenceDied.set("Occurrence Died", ofFloatColor(0.766254, 0.644068, 0.452822, 0.792816)));
  add(occurrenceSpawnFailed.set("Occurrence Spawn Failed", ofFloatColor(0.814066, 0.681255, 0.511258, 0.569695)));
}
