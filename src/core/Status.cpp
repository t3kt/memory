//
//  Status.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Status.h"
#include <ofMain.h>

const float LABEL_WIDTH = 150;
const float VALUE_WIDTH = 60;
const float PADDING = 5;
const float TOP = 5;
const float RIGHT = 5;
const float LINE_HEIGHT = 15;

std::size_t StatusInfo::registerLine(std::string name) {
  _lines.push_back(std::pair<std::string, std::string>(name, ""));
  return _lines.size() - 1;
}

void StatusInfo::setValue(std::size_t index, std::string value) {
  _lines[index].second = value;
}

void StatusInfoController::setup() {
  _text.load(OF_TTF_MONO, 12);
  _text.setMinHeight(LINE_HEIGHT);
}

void StatusInfoController::draw() {
  float lineHeight = LINE_HEIGHT;
  float y = TOP;
  float xValue = ofGetWidth() - VALUE_WIDTH - RIGHT;
  float xLabel = xValue - PADDING;

  StatusInfo::StatusList allLines;

  for (auto provider : _providers) {
    const StatusInfo& status = provider->getStatusInfo();
    const StatusInfo::StatusList& lines = status.getLines();
    allLines.insert(allLines.end(), lines.begin(), lines.end());
  }

  ofPushStyle();
  ofFill();
  ofSetColor(ofFloatColor(0.5, 0.5, 0.5, 0.8));
  ofSetRectMode(OF_RECTMODE_CORNER);
  ofDrawRectangle(xLabel - LABEL_WIDTH,
                  y - PADDING,
                  LABEL_WIDTH + PADDING + VALUE_WIDTH,
                  lineHeight * allLines.size() + PADDING * 2);
  ofPopStyle();

  ofPushStyle();
  ofSetColor(ofFloatColor::white);

  for (const auto& line : allLines) {
    _text.setMaxWidth(LABEL_WIDTH);
    _text.draw(line.first,
               xLabel, y,
               ofxTextAlign::HORIZONTAL_ALIGN_RIGHT
               | ofxTextAlign::VERTICAL_ALIGN_TOP);
    _text.setMaxWidth(VALUE_WIDTH);
    _text.draw(line.second,
               xValue, y,
               ofxTextAlign::HORIZONTAL_ALIGN_LEFT
               | ofxTextAlign::VERTICAL_ALIGN_TOP);
    y += lineHeight;
  }

  ofPopStyle();
}
