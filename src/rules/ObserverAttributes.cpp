//
//  ObserverAttributes.cpp
//  memory
//
//  Created by tekt on 9/25/16.
//
//

#include "../app/AppSystem.h"
#include "../core/Context.h"
#include "../rules/ObserverAttributes.h"

ObserverAttributes::ObserverAttributes(Context& context,
                                       const Params& params)
: _params(params)
, _colorTheme(AppSystem::get().params()->colors)
, _context(context) { }

void ObserverAttributes::setup() {
  _color =
  std::make_shared<EntityAttribute<ObserverEntity, ofFloatColor>>(_colorTheme.observerMarker.get());
  _color->addRule([&](const ofFloatColor& prevColor,
                      std::shared_ptr<ObserverEntity>& entity) {
    //...
    return prevColor;
  });
}

void ObserverAttributes::update() {
  for (auto& entity : _context.observers) {
//    entity->setCo
  }
}
