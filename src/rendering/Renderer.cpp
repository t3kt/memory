//
//  Renderer.cpp
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#include "Renderer.h"
#include <ofMain.h>

ObserverRenderer::Params::Params()
: ::Params() {
  add(drawRadius
      .setKey("drawRadius")
      .setName("Draw Radius")
      .setValueAndDefault(0.03)
      .setRange(0, 0.1));
}

ObserverRenderer::ObserverRenderer(const ObserverRenderer::Params& params,
                                   const ColorTheme& colors,
                                   ObjectManager<ObserverEntity>& observers)
: _params(params)
, _colors(colors)
, _observers(observers) { }

void ObserverRenderer::setup() {

}

void ObserverRenderer::update(const State &state) {

}

void ObserverRenderer::draw(const State &state) {
  ofFloatColor baseColor = _colors.observerMarker.get();
  float radius = _params.drawRadius.get();
  ofPushStyle();
  ofFill();
  for (shared_ptr<ObserverEntity> observer : _observers) {
    if (!observer->visible()) {
      continue;
    }
    float alpha = observer->getRemainingLifetimeFraction();
    if (alpha <= 0) {
      continue;
    }
    ofSetColor(ofFloatColor(baseColor, baseColor.a * alpha));
    ofDrawSphere(observer->position(), radius);
  }
  //...
  ofPopStyle();
}
