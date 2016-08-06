//
//  Status.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include <ofMain.h>
#include "Status.h"

const float LABEL_WIDTH = 150;
const float VALUE_WIDTH = 60;
const float PADDING = 5;
const float TOP = 5;
const float RIGHT = 5;
const float LINE_HEIGHT = 15;

StatusInfoController::StatusInfoController(const Context& context)
: _context(context) {
  _text.load(OF_TTF_MONO, 12);
  _text.setMinHeight(LINE_HEIGHT);
}

void StatusInfoController::draw() {
  float y = TOP;
  float xValue = ofGetWidth() - VALUE_WIDTH - RIGHT;
  float xLabel = xValue - PADDING;

  std::vector<std::pair<std::string, std::string>> lines;

  lines.emplace_back("State:",
                     _context.state.running ? "Playing" : "Paused");
  lines.emplace_back("Time:", ofToString(_context.time(), 2));
  lines.emplace_back("FPS:", ofToString(ofGetFrameRate(), 2));
  lines.emplace_back("Observers:", ofToString(_context.observers.size()));
  lines.emplace_back("Occurrences:", ofToString(_context.occurrences.size()));
  lines.emplace_back("Animations:", ofToString(_context.animations.size()));
  lines.emplace_back("Navigators:", ofToString(_context.navigators.size()));

  ofPushStyle();
  ofFill();
  ofSetColor(ofFloatColor(0.5, 0.5, 0.5, 0.8));
  ofSetRectMode(OF_RECTMODE_CORNER);
  ofDrawRectangle(xLabel - LABEL_WIDTH,
                  y - PADDING,
                  LABEL_WIDTH + PADDING + VALUE_WIDTH,
                  LINE_HEIGHT * lines.size() + PADDING * 2);
  ofPopStyle();

  ofPushStyle();
  ofSetColor(ofFloatColor::white);

  for (const auto& line : lines) {
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
    y += LINE_HEIGHT;
  }

  ofPopStyle();
}
