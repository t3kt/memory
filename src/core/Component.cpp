//
//  Component.cpp
//  memory
//
//  Created by tekt on 10/5/16.
//
//

#include "../core/Component.h"

void MultiComponentCollection::update() {
  for (auto& component : _updates) {
    component->update();
  }
}

void MultiComponentCollection::draw() {
  for (auto& component : _draws) {
    component->draw();
  }
}
