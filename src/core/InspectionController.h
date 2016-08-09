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
#include "Info.h"
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
      add(showInfo
          .setKey("showInfo")
          .setName("Show Info")
          .setValueAndDefault(false));
      setEnabledValueAndDefault(false);
    }
    TParam<float> clickRange;
    TParam<bool> showInfo;
  };

  InspectionController(Params& params,
                       Context& context,
                       ofCamera& camera,
                       ofAppGLFWWindow& window)
  : _params(params)
  , _context(context)
  , _camera(camera)
  , _window(window)
  , _infoBox(InfoBox::Position::BOTTOM_RIGHT) { }

  void setup();
  void update();
  void draw();
private:
  void onMousePressed(ofMouseEventArgs& event);
  void attachToEvents();
  void detachFromEvents();
  void onEnabledChanged(bool enabled);
  void updateHighlights();
  void updateInfo();
  void drawInfo();

  Params& _params;
  Context& _context;
  ofCamera& _camera;
  ofAppGLFWWindow& _window;

  std::shared_ptr<ParticleObject> _selectedEntity;
  ofVec3f _selectedScreenPosition;
  ofVec2f _clickPos;
  bool _hasClick;

  Info _info;
  InfoBox _infoBox;
};

#endif /* InspectionController_h */
