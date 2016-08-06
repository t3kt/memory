//
//  InspectionController.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofGraphics.h>
#include "Context.h"
#include "InspectionController.h"

void InspectionController::setup() {
  _params.enabled.changed += [&](bool& enabled) {
    onEnabledChanged(enabled);
  };
  onEnabledChanged(_params.enabled.get());
}

void InspectionController::onEnabledChanged(bool enabled) {
  if (enabled) {
    attachToEvents();
  } else {
    detachFromEvents();
  }
}

void InspectionController::attachToEvents() {
  auto& coreEvents = _window.events();
  ofAddListener(coreEvents.mousePressed,
                this,
                &InspectionController::onMousePressed);
}

void InspectionController::detachFromEvents() {
  auto& coreEvents = _window.events();
  ofRemoveListener(coreEvents.mousePressed,
                   this,
                   &InspectionController::onMousePressed);
}

// must be called after camera has ended
void InspectionController::update() {
  if (!_params.enabled.get()) {
    return;
  }
  if (_hasClick) {
    _camera.begin();
    float closestDist = -1;
    float range = _params.clickRange.get();
    _context.performActionOnParticleEntityPtrs([&](std::shared_ptr<ParticleObject> entity) {
      ofVec3f screenPos = _camera.worldToScreen(entity->position());
      float dist = screenPos.distance(_clickPos);
      if (dist > range) {
        return;
      }
      if (closestDist < 0 || dist < closestDist) {
        closestDist = dist;
        _selectedEntity = entity;
      }
    });
    _hasClick = false;
    _camera.end();
  }
  if (_selectedEntity) {
    if (!_selectedEntity->alive()) {
      _selectedEntity.reset();
    } else {
      _selectedScreenPosition = _camera.worldToScreen(_selectedEntity->position());
    }
  }
}

// must be called after camera has ended
void InspectionController::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  if (_selectedEntity) {
    ofPushStyle();
//    ofPushMatrix();
    ofSetColor(ofFloatColor::white);
    auto winSize = _window.getWindowSize();
    const auto& pos = _selectedScreenPosition;
    ofVec3f sidePos(0, pos.y);
    if (pos.x > winSize.x / 2) {
      sidePos.x = winSize.x;
    }
    ofDrawLine(pos, sidePos);
//    ofNoFill();
//    ofSetRectMode(OF_RECTMODE_CENTER);
//    ofDrawRectangle(pos, 10, 10);
    //...
//    ofPopMatrix();
    ofPopStyle();
  }
}

void InspectionController::onMousePressed(ofMouseEventArgs &event) {
  _clickPos = event;
  _hasClick = true;
}
