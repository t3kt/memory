//
//  Renderer.h
//  memory
//
//  Created by tekt on 7/2/16.
//
//

#ifndef Renderer_h
#define Renderer_h

#include "Params.h"
#include "Colors.h"
#include "State.h"
#include "OccurrenceEntity.h"
#include "ObserverEntity.h"
#include "ObjectManager.h"

class ObserverRenderer {
public:
  class Params : public ::Params {
  public:
    Params();

    Json to_json() const override;
    void read_json(const Json& obj) override;

    TParam<float> drawRadius;
  };

  ObserverRenderer(const Params& params,
                   const ColorTheme& colors,
                   ObjectManager<ObserverEntity>& observers);

  void setup();
  void update(const State& state);
  void draw(const State& state);

private:
  const Params& _params;
  const ColorTheme& _colors;
  ObjectManager<ObserverEntity>& _observers;
};

#endif /* Renderer_h */
