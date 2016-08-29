//
//  AppActions.cpp
//

#include "../app/AppActions.h"
#include "../app/AppSystem.h"

EnumTypeInfo<AppAction> AppActionType({
  {"(none)", AppAction::NONE},
  {"resetCamera", AppAction::RESET_CAMERA},
  {"toggleLogging", AppAction::TOGGLE_LOGGING},
  {"toggleClockState", AppAction::TOGGLE_CLOCK_STATE},
  {"spawnFewOccurrences", AppAction::SPAWN_FEW_OCCURRENCES},
  {"spawnFewObservers", AppAction::SPAWN_FEW_OBSERVERS},
  {"spawnManyOccurrences", AppAction::SPAWN_MANY_OCCURRENCES},
  {"spawnManyObservers", AppAction::SPAWN_MANY_OBSERVERS},
  {"spawnTonsOfObservers", AppAction::SPAWN_TONS_OF_OBSERVERS},
  {"spawnTonsOfOccurrences", AppAction::SPAWN_TONS_OF_OCCURRENCES},
  {"spawnLoadTestEntities", AppAction::SPAWN_LOAD_TEST_ENTITIES},
  {"killFewObservers", AppAction::KILL_FEW_OBSERVERS},
  {"killManyObservers", AppAction::KILL_MANY_OBSERVERS},
  {"loadSettings", AppAction::LOAD_SETTINGS},
  {"saveSettings", AppAction::SAVE_SETTINGS},
  {"stopAllEntities", AppAction::STOP_ALL_ENTITIES},
  {"toggleShowPhysics", AppAction::TOGGLE_SHOW_PHYSICS},
  {"toggleShowBounds", AppAction::TOGGLE_SHOW_BOUNDS},
  {"dumpEntityState", AppAction::DUMP_ENTITY_STATE},
  {"loadEntityState", AppAction::LOAD_ENTITY_STATE},
  {"saveEntityState", AppAction::SAVE_ENTITY_STATE},
});

std::ostream& operator<<(std::ostream& os, const AppAction& action) {
  return os << AppActionType.toString(action);
}

void AppActionHandler::registerAsActionHandler() {
  AppSystem::get().appActionTriggered += [&](AppActionEventArgs& event) {
    if (performAction(event.value())) {
      event.markHandled();
    }
  };
}
