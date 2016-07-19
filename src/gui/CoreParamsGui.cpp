//
//  CoreParamsGui.cpp
//  memory
//
//  Created by tekt on 7/18/16.
//
//

#include "CoreParamsGui.h"

ofxControlToggle* DebugParamsGui::addToggle(TParam<bool> &param,
                                            bool &storage) {
  return _root.addToggle(param.getName(),
                         &storage,
                         this,
                         &DebugParamsGui::onToggleEvent);
}

void DebugParamsGui::onToggleEvent(ofxControlButtonEventArgs& event) {
  if (event.button == _loggingEnabledToggle) {
    _params.loggingEnabled.set(event.value);
  }
}

void DebugParamsGui::setup(ofxControlWidget& parent) {
  parent.addWidget(&_root);
  _loggingEnabledToggle = addToggle(_params.loggingEnabled,
                                    _storage.loggingEnabled);
  _showBoundsToggle = addToggle(_params.showBounds,
                                _storage.showBounds);
  _showStatusToggle = addToggle(_params.showStatus,
                                _storage.showStatus);
  _showPhysicsToggle = addToggle(_params.showPhysics,
                                 _storage.showPhysics);
//  _inspectToggle = addToggle(_params.inspect,
//                             _storage.inspect);
}
