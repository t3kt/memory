//
//  AppActions.h
//
//  Actions which can be triggered by things like keyboard shortcuts
//  and handled by various components.
//

#ifndef AppActions_h
#define AppActions_h

#include <iostream>
#include "../core/Common.h"
#include "../core/Enums.h"
#include "../core/Events.h"

// Type of action
enum class AppAction {
  NONE,
  RESET_CAMERA,
  TOGGLE_LOGGING,
  TOGGLE_CLOCK_STATE,
  SPAWN_FEW_OCCURRENCES,
  SPAWN_FEW_OBSERVERS,
  SPAWN_MANY_OCCURRENCES,
  SPAWN_MANY_OBSERVERS,
  SPAWN_TONS_OF_OBSERVERS,
  SPAWN_TONS_OF_OCCURRENCES,
  SPAWN_LOAD_TEST_ENTITIES,
  SPAWN_OBSERVER_NAVIGATOR,
  KILL_FEW_OBSERVERS,
  KILL_FEW_OCCURRENCES,
  KILL_MANY_OBSERVERS,
  KILL_MANY_OCCURRENCES,
  LOAD_SETTINGS,
  SAVE_SETTINGS,
  STOP_ALL_ENTITIES,
  TOGGLE_SHOW_PHYSICS,
  TOGGLE_SHOW_BOUNDS,
  DUMP_ENTITY_STATE,
  SAVE_ENTITY_STATE,
  LOAD_ENTITY_STATE,
  TEST_ACTION,
  RELOAD_THEME,
  SELECT_PREV_OBSERVER,
  SELECT_NEXT_OBSERVER,
};

using AppActionEventArgs = ValueEventArgs<AppAction>;
using AppActionEvent = ValueEvent<AppAction>;

// A component which can handle AppActions
class AppActionHandler {
public:
  AppActionHandler();

  virtual bool performAction(AppAction action) = 0;
};

std::ostream& operator<<(std::ostream& os, const AppAction& action);

#endif /* AppActions_h */
