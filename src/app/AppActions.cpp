//
//  AppActions.cpp
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#include "AppActions.h"

EnumTypeInfo<AppAction> AppActionType({
  {"resetCamera", AppAction::RESET_CAMERA},
  {"toggleLogging", AppAction::TOGGLE_LOGGING},
  {"toggleClockState", AppAction::TOGGLE_CLOCK_STATE},
  {"spawnFewOccurrences", AppAction::SPAWN_FEW_OCCURRENCES},
  {"spawnFewObservers", AppAction::SPAWN_FEW_OBSERVERS},
  {"spawnManyOccurrences", AppAction::SPAWN_MANY_OCCURRENCES},
  {"spawnManyObservers", AppAction::SPAWN_MANY_OBSERVERS},
  {"killFewObservers", AppAction::KILL_FEW_OBSERVERS},
  {"killManyObservers", AppAction::KILL_MANY_OBSERVERS},
  {"loadSettings", AppAction::LOAD_SETTINGS},
  {"saveSettings", AppAction::SAVE_SETTINGS},
  {"stopAllEntities", AppAction::STOP_ALL_ENTITIES},
  {"toggleShowPhysics", AppAction::TOGGLE_SHOW_PHYSICS},
  {"toggleShowBounds", AppAction::TOGGLE_SHOW_BOUNDS},
});

