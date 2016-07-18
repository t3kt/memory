//
//  InspectionController.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef InspectionController_h
#define InspectionController_h

#include <memory>
#include <ofAppGLFWWindow.h>
#include <ofCamera.h>
#include <ofEvents.h>
#include "Params.h"
#include "ParticleObject.h"

class Context;

class InspectionController {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(clickRange
          .setKey("clickRange")
          .setName("Click Range")
          .setValueAndDefault(10)
          .setRange(0, 100));
    }
    TParam<float> clickRange;
  };

  InspectionController(Params& params,
                       Context& context,
                       ofCamera& camera,
                       ofAppGLFWWindow& window)
  : _params(params)
  , _context(context)
  , _camera(camera)
  , _window(window){ }

  void setup();
  void update();
  void draw();
private:
  void onMousePressed(ofMouseEventArgs& event);
  void attachToEvents();
  void detachFromEvents();
  void onEnabledChanged(bool enabled);

  Params& _params;
  Context& _context;
  ofCamera& _camera;
  ofAppGLFWWindow& _window;

  std::shared_ptr<ParticleObject> _selectedEntity;
  ofVec3f _selectedScreenPosition;
  ofVec2f _clickPos;
  bool _hasClick;
};

#endif /* InspectionController_h */
