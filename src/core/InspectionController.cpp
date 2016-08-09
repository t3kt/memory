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
    _context.highlightedEntities.clear();
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

// must not be called while camera is active
void InspectionController::update() {
  if (!_params.enabled.get()) {
    return;
  }
  if (_hasClick) {
    _camera.begin();
    float closestDist = -1;
    float range = _params.clickRange.get();
    bool found = false;
    _context.performActionOnParticleEntityPtrs([&](std::shared_ptr<ParticleObject> entity) {
      ofVec3f screenPos = _camera.worldToScreen(entity->position());
      float dist = screenPos.distance(_clickPos);
      if (dist > range) {
        return;
      }
      if (closestDist < 0 || dist < closestDist) {
        closestDist = dist;
        _selectedEntity = entity;
        found = true;
      }
    });
    if (!found) {
      _selectedEntity.reset();
    }
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
  updateHighlights();
  updateInfo();
}

void InspectionController::updateHighlights() {
  _context.highlightedEntities.clear();
  if (!_selectedEntity) {
    return;
  }
  _context.highlightedEntities.add(_selectedEntity);
  _selectedEntity->performActionOnConnected([&](std::shared_ptr<WorldObject> entity) {
    _context.highlightedEntities.add(entity);
  });
}

void InspectionController::updateInfo() {
  _info.clear();
  if (!_params.showInfo.get()) {
    return;
  }
  if (!_selectedEntity || !_selectedEntity->alive()) {
    return;
  }

  _selectedEntity->fillInfo(_info);
}

// must not be called while camera is active
void InspectionController::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  if (_selectedEntity) {
    ofPushStyle();
    ofSetColor(ofFloatColor::white);
    auto winSize = _window.getWindowSize();
    const auto& pos = _selectedScreenPosition;
    ofVec3f sidePos(0, pos.y);
//    if (pos.x > winSize.x / 2) {
      sidePos.x = winSize.x;
//    }
    ofDrawLine(pos, sidePos);
    ofPopStyle();

    drawInfo();
  }
}

void InspectionController::drawInfo() {
  if (!_params.showInfo.get()) {
    return;
  }
  _infoBox.drawInfo(_info);
}

void InspectionController::onMousePressed(ofMouseEventArgs &event) {
  if (event.button != OF_MOUSE_BUTTON_3) {
    return;
  }
  _clickPos = event;
  _hasClick = true;
}
