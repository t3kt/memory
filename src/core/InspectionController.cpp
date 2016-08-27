//
//  InspectionController.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofGraphics.h>
#include <set>
#include "../core/Context.h"
#include "../core/InspectionController.h"

struct ParticleScreenDepthLess {
  constexpr bool operator()(const ParticlePtr& lha,
                            const ParticlePtr& rha) const {
    return lha->screenPos().z < rha->screenPos().z;
  }
};

using ScreenDepthSortedParticleSet =
std::set<ParticlePtr, ParticleScreenDepthLess>;

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

    ScreenDepthSortedParticleSet candidates;

    auto range = _params.clickRange.get();
    _context.performActionOnParticleEntityPtrs([&](ParticlePtr entity) {
      const auto& screenPos = entity->screenPos();
      auto xyPos = ofVec2f(screenPos.x, screenPos.y);
      float xyDist = xyPos.distance(_clickPos);
      if (xyDist > range) {
        return;
      }
      candidates.insert(entity);
    });

    if (candidates.empty()) {
      _selectedEntity.reset();
    } else {
      _selectedEntity = *candidates.begin();
    }

    _hasClick = false;
  }
  if (_selectedEntity && !_selectedEntity->alive()) {
    _selectedEntity.reset();
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
    const auto& pos = _selectedEntity->screenPos();
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
