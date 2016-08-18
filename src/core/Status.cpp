//
//  Status.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include <ofMain.h>
#include "../core/Status.h"

StatusInfoController::StatusInfoController(const Context& context)
: _context(context)
, _infoBox(InfoBox::Position::TOP_RIGHT) { }

void StatusInfoController::update() {
  _info.clear();
  _info.add("State:", _context.state.running ? "Playing" : "Paused");
  _info.add("Time:", ofToString(_context.time(), 2));
  _info.add("FPS:", ofToString(ofGetFrameRate(), 2));
  _info.add("Observers:", ofToString(_context.state.stats.observers.living));
  _info.add("Occurrences:", ofToString(_context.state.stats.occurrences.living));
  _info.add("Animations:", ofToString(_context.state.stats.animations.living));
  _info.add("Navigators:", ofToString(_context.state.stats.navigators.living));
  if (!_context.highlightedEntities.empty()) {
    _info.add("Highlighted:",
              ofToString(_context.highlightedEntities.size()));
  }
}

void StatusInfoController::draw() {
  _infoBox.drawInfo(_info);
}
