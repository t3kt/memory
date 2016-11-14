//
//  ScenesController.h
//

#pragma once

#include "../core/Component.h"
#include "../scenes/Scenes.h"

class ActionsController;
class Context;

class ScenesController
: public ComponentBase {
public:
  ScenesController(Context& context, ActionsController& actions)
  : _context(context)
  , _actions(actions) { }

  void update() override;

private:
  Context& _context;
  ActionsController& _actions;
};
