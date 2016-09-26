//
//  ObserverAttributes.cpp
//  memory
//
//  Created by tekt on 9/25/16.
//
//

#include "../app/AppSystem.h"
#include "../core/Context.h"
#include "../core/ObserversController.h"
#include "../rendering/ObserverPreRenderer.h"
#include "../rendering/ObserverRenderer.h"
#include "../rules/ObserverAttributes.h"

class ObserverHighlightDarkenRule
: public EntityRule<ObserverEntity, ofFloatColor> {
public:
  ObserverHighlightDarkenRule(const ObserverRenderingParams& params)
  : _highlightAmount(params.renderer.highlightAmount.get()) { }
  
private:
  const float& _highlightAmount;
};

ObserverAttributes::ObserverAttributes(Context& context,
                                       const ObserverParams& params,
                                       const ObserverRenderingParams& renderParams)
: _params(params)
, _renderParams(renderParams)
, _colorTheme(AppSystem::get().params()->colors)
, _context(context) { }

void ObserverAttributes::setup() {
  _color =
  std::make_shared<EntityAttribute<ObserverEntity, ofFloatColor>>(_colorTheme.observerMarker.get());
  _color->addRule([&](const ofFloatColor& prevColor,
                      std::shared_ptr<ObserverEntity>& entity) {
    auto alpha = entity->getRemainingLifetimeFraction();
    auto age = entity->getAge(_context.state);
    //...
    return prevColor;
  });
}

void ObserverAttributes::update() {
  for (auto& entity : _context.observers) {
//    entity->setCo
  }
}
