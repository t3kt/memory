//
//  InspectionController.h
//

#pragma once

#include <memory>
#include <ofAppGLFWWindow.h>
#include <ofEvents.h>
#include <ofxTCommon.h>
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/Info.h"
#include "../core/ParticleObject.h"

class Context;

// Controller that supports selecting an entity and viewing details
// about it.
class InspectionController
: public ofxTCommon::NonCopyable
, public ComponentBase {
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
                       ofAppGLFWWindow& window)
  : _params(params)
  , _context(context)
  , _window(window)
  , _infoBox(InfoBox::Position::BOTTOM_RIGHT) { }

  void setup() override;
  void update() override;
  void draw() override;
private:
  void onMousePressed(ofMouseEventArgs& event);
  void attachToEvents();
  void detachFromEvents();
  void onEnabledChanged(bool enabled);
  void updateHighlights();
  void updateInfo();
  void drawInfo();

  template<typename E>
  void selectPrevEntity();

  template<typename E>
  void selectNextEntity();

  bool isTypeSelected(EntityType type) const;

  Params& _params;
  Context& _context;
  ofAppGLFWWindow& _window;

  ParticlePtr _selectedEntity;
  ofVec2f _clickPos;
  bool _hasClick;

  Info _info;
  InfoBox _infoBox;
};

