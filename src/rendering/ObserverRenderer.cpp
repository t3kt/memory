//
//  ObserverRenderer.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <ofMain.h>
#include "../app/AppAssets.h"
#include "../app/AppSystem.h"
#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../rendering/ObserverRenderer.h"

ObserverRenderer::ObserverRenderer(const Params& params,
                                   const ColorTheme& colors,
                                   Context& context)
: _context(context)
, _entities(context.observers)
, _params(params)
, _colors(colors) { }

void ObserverRenderer::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  auto renderer = ofGetCurrentRenderer();

  renderer->pushStyle();

  auto darkening = 1.0 - _params.highlightAmount.get();
  auto baseColor = _colors.observerMarker.get();
  auto darkenedColor = ofFloatColor(baseColor, baseColor.a * darkening);
  auto hasHighlights = !_context.highlightedEntities.empty();

  float size = _params.size.get();

  for (const auto& entity : _entities) {
    if (!entity->visible()) {
      continue;
    }

    ofFloatColor color;
    if (hasHighlights &&
        !_context.highlightedEntities.containsId(entity->id())) {
      color = darkenedColor;
    } else {
      color = baseColor;
    }
    color.a *= entity->alpha();
    renderer->setColor(color);

    renderer->drawSphere(entity->position(), size);
  }

  renderer->popStyle();
}

static const std::size_t MAX_OBSERVERS = 1000;

InstancedObserverRenderer::InstancedObserverRenderer(const Params& params,
                                                     const ColorTheme& colors,
                                                     Context& context)
: _params(params)
, _context(context)
, _colors(colors)
, _mesh(AppAssets::observerMarkerMesh())
, _instanceShader(AppAssets::markerInstanceShader())
, _fadeIn(params.fadeIn) { }

void InstancedObserverRenderer::setup() {

  _matrices.resize(MAX_OBSERVERS);
  _matricesBuffer.allocate();
  _matricesBuffer.bind(GL_TEXTURE_BUFFER);
  _matricesBuffer.setData(_matrices, GL_STREAM_DRAW);
  _matricesTexture.allocateAsBufferTexture(_matricesBuffer, GL_RGBA32F);

  auto& colors = _mesh.getColors();
  colors.resize(MAX_OBSERVERS);
  ofFloatColor zeroColor;
  colors.assign(MAX_OBSERVERS, zeroColor);

  _mesh.setUsage(GL_STATIC_DRAW);
  _mesh.getVbo().setAttributeDivisor(ofShader::COLOR_ATTRIBUTE, 1);

  _instanceShader.begin();
  _instanceShader.setUniformTexture("matricesTex", _matricesTexture, 0);
  _instanceShader.end();
}

void InstancedObserverRenderer::update() {
  if (!_params.enabled.get()) {
    return;
  }
  auto baseColor = _colors.observerMarker.get();
  float size = _params.size.get();
  auto fadeIn = _fadeIn.getPhrase();
  const auto& entities = _context.observers;
  auto& colors = _mesh.getColors();

  auto count = entities.size();
  if (count > MAX_OBSERVERS) {
    AppSystem::get().log().observer().logWarning([&](ofLog& log) {
      log << "Exceeded maximum observer count: " << count << " (limit: " << MAX_OBSERVERS << ")";
    });
    count = MAX_OBSERVERS;
  }

  std::size_t i = 0;
  for (const auto& entity : entities) {
    float alpha = entity->getRemainingLifetimeFraction();
    float age = entity->getAge(_context.state);
    if (age < fadeIn->getDuration()) {
      alpha *= fadeIn->getValue(age);
    }
    if (alpha <= 0) {
      colors[i].a = 0;
    } else {
      colors[i] = ofFloatColor(baseColor, baseColor.a * alpha);
//      _matrices[i] = ofMatrix4x4::newTranslationMatrix(entity->position());
      ofNode node;
      node.setPosition(entity->position());
      node.setScale(size);
      _matrices[i] = node.getLocalTransformMatrix();
    }
    ++i;
  }
  
  _matricesBuffer.updateData(0, _matrices);
}

void InstancedObserverRenderer::draw() {
  if (!_params.enabled.get()) {
    return;
  }
  _instanceShader.begin();
  _mesh.drawInstanced(OF_MESH_FILL, _count);
  _instanceShader.end();
}
