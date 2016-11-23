//
//  ScenesController.h
//

#pragma once

#include <ofxTCommon.h>
#include "../core/Component.h"
#include "../scenes/Scenes.h"

class ActionsController;
class Context;

class ScenesController
: public ComponentBase
, public ofxTCommon::NonCopyable {
public:
  ScenesController(Context& context, ActionsController& actions)
  : _context(context)
  , _actions(actions) { }

  void setup() override;

  void update() override;

  void loadSceneFile(const std::string& filename);

  void startScene(std::shared_ptr<Scene> scene);

  void promptAndLoadScene();

private:
  Context& _context;
  ActionsController& _actions;
};
