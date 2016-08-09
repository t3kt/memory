//
//  Info.cpp
//  memory
//
//  Created by tekt on 8/8/16.
//
//

#include <ofMain.h>
#include "Info.h"

const float LINE_HEIGHT = 15;
const float CHAR_WIDTH = 10;
const float PADDING = 5;

InfoBox::InfoBox(Position position)
: _position(position) {
  _text.load(OF_TTF_MONO, 12);
  _text.setMinHeight(LINE_HEIGHT);
}

ofRectangle InfoBox::getRect(const Info &info) {
  float height = info.size() * LINE_HEIGHT + PADDING + PADDING;
  float width = (info.labelMaxLength() + info.valueMaxLength()) * CHAR_WIDTH + PADDING + PADDING + PADDING;
  float left = ofGetWidth() - width - PADDING;
  float top;
  switch (_position) {
    case Position::BOTTOM_RIGHT:
      top = ofGetHeight() - height - PADDING;
      break;
    case Position::TOP_RIGHT:
    default:
      top = PADDING;
      break;
  }
  return ofRectangle(left, top, width, height);
}

void InfoBox::drawInfo(const Info &info) {
  if (info.empty()) {
    return;
  }
  auto rect = getRect(info);
  ofPushStyle();
  ofSetColor(ofFloatColor(0.5, 0.5, 0.5, 0.8));
  ofSetRectMode(OF_RECTMODE_CORNER);
  ofDrawRectangle(rect);

  ofSetColor(ofFloatColor::white);

  float y = rect.y + PADDING;
  float labelWidth = info.labelMaxLength() * CHAR_WIDTH;
  float valueWidth = info.valueMaxLength() * CHAR_WIDTH;
  float xValue = rect.getRight() - PADDING - valueWidth;
  float xLabel = xValue - PADDING;
  for (const auto& line : info) {
    _text.setMaxWidth(labelWidth);
    _text.draw(line.first,
               xLabel, y,
               ofxTextAlign::HORIZONTAL_ALIGN_RIGHT
               | ofxTextAlign::VERTICAL_ALIGN_TOP);
    _text.setMaxWidth(valueWidth);
    _text.draw(line.second,
               xValue, y,
               ofxTextAlign::HORIZONTAL_ALIGN_LEFT
               | ofxTextAlign::VERTICAL_ALIGN_TOP);
    y += LINE_HEIGHT;
  }
  ofPopStyle();
}

template<>
void Info::add(const std::string& label, const float& value) {
  add(label, ofToString(value, 2));
}

template<>
void Info::add(const std::string& label, const ofVec3f& value) {
  add(label, ofToString(value, 2));
}
