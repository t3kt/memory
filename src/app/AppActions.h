//
//  AppActions.h
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#ifndef AppActions_h
#define AppActions_h

#include <iostream>
#include "Common.h"
#include "Events.h"

enum class AppAction {
  NONE,
  RESET_CAMERA,
  TOGGLE_LOGGING,
  TOGGLE_CLOCK_STATE,
  SPAWN_FEW_OCCURRENCES,
  SPAWN_FEW_OBSERVERS,
  SPAWN_MANY_OCCURRENCES,
  SPAWN_MANY_OBSERVERS,
  KILL_FEW_OBSERVERS,
  KILL_MANY_OBSERVERS,
  LOAD_SETTINGS,
  SAVE_SETTINGS,
  STOP_ALL_ENTITIES,
  TOGGLE_SHOW_PHYSICS,
  TOGGLE_SHOW_BOUNDS,
};

using AppActionEventArgs = ValueEventArgs<AppAction>;
using AppActionEvent = TEvent<AppActionEventArgs>;

extern EnumTypeInfo<AppAction> AppActionType;

class AppActionHandler {
public:
  virtual bool performAction(AppAction action) = 0;

protected:
  void registerWithAppSystem();
};

std::ostream& operator<<(std::ostream& os, const AppAction& action);

#endif /* AppActions_h */
