//
//  Status.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include <ofMain.h>
#include "../app/AppParameters.h"
#include "../control/ParamTransition.h"
#include "../core/Status.h"

StatusInfoController::StatusInfoController(const Params& params,
                                           const Context& context)
: _params(params)
, _context(context)
, _infoBox(InfoBox::Position::TOP_RIGHT) { }

void StatusInfoController::update() {
  if (!_params.showStatus.get()) {
    return;
  }
  _info.clear();
  _info.add("State:", _context.rootState.running ? "Playing" : "Paused");
  _info.add("Time:", ofToString(_context.time(), 2));
  _info.add("FPS:", ofToString(ofGetFrameRate(), 2));
  _info.add("Observers:", _context.observers.size());
  _info.add("Occurrences:", _context.occurrences.size());
  _info.add("Animations:", _context.animations.size());
  _info.add("Navigators:", _context.navigators.size());
  _info.add("Nodes:", _context.nodes.size());
  if (!_context.highlightedEntities.empty()) {
    _info.add("Highlighted:",
              _context.highlightedEntities.size());
  }
  if (_context.activeTransition) {
    _info.add("Transitioning:",
              _context.activeTransition->name());
  }
}

void StatusInfoController::draw() {
  if (!_params.showStatus.get()) {
    return;
  }
  _infoBox.drawInfo(_info);
}
