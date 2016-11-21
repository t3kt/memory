//
//  AppActions.h
//
//  Actions which can be triggered by things like keyboard shortcuts
//  and handled by various components.
//

#pragma once

#include <iostream>
#include <ofxTEvents.h>
#include "../core/Common.h"

// Type of action
enum class AppAction {
  NONE,
  SPAWN_FEW_OCCURRENCES,
  SPAWN_FEW_OBSERVERS,
  SPAWN_MANY_OCCURRENCES,
  SPAWN_MANY_OBSERVERS,
  SPAWN_TONS_OF_OBSERVERS,
  SPAWN_TONS_OF_OCCURRENCES,
  SPAWN_LOAD_TEST_ENTITIES,
  SPAWN_OBSERVER_NAVIGATOR,
  LOAD_SCENE,
};

using AppActionEventArgs = ofxTCommon::ValueEventArgs<AppAction>;
using AppActionEvent = ofxTCommon::ValueEvent<AppAction>;

// A component which can handle AppActions
class AppActionHandler {
public:
  AppActionHandler();

  virtual bool performAction(AppAction action) = 0;
};

std::ostream& operator<<(std::ostream& os, const AppAction& action);

