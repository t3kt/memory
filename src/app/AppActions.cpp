//
//  AppActions.cpp
//

#include <ofxTEnums.h>
#include "../app/AppActions.h"
#include "../app/AppSystem.h"

using namespace ofxTCommon;

EnumTypeInfo<AppAction> AppActionType({
  {"(none)", AppAction::NONE},
  {"spawnFewOccurrences", AppAction::SPAWN_FEW_OCCURRENCES},
  {"spawnFewObservers", AppAction::SPAWN_FEW_OBSERVERS},
  {"spawnManyOccurrences", AppAction::SPAWN_MANY_OCCURRENCES},
  {"spawnManyObservers", AppAction::SPAWN_MANY_OBSERVERS},
  {"spawnTonsOfObservers", AppAction::SPAWN_TONS_OF_OBSERVERS},
  {"spawnTonsOfOccurrences", AppAction::SPAWN_TONS_OF_OCCURRENCES},
  {"spawnLoadTestEntities", AppAction::SPAWN_LOAD_TEST_ENTITIES},
  {"spawnObserverNavigator", AppAction::SPAWN_OBSERVER_NAVIGATOR},
  {"loadSettings", AppAction::LOAD_SETTINGS},
  {"saveSettings", AppAction::SAVE_SETTINGS},
  {"stopAllEntities", AppAction::STOP_ALL_ENTITIES},
  {"toggleShowPhysics", AppAction::TOGGLE_SHOW_PHYSICS},
  {"toggleShowBounds", AppAction::TOGGLE_SHOW_BOUNDS},
  {"dumpEntityState", AppAction::DUMP_ENTITY_STATE},
  {"loadEntityState", AppAction::LOAD_ENTITY_STATE},
  {"saveEntityState", AppAction::SAVE_ENTITY_STATE},
  {"reloadTheme", AppAction::RELOAD_THEME},
  {"toggleFullscreen", AppAction::TOGGLE_FULLSCREEN},
  {"capturePreset", AppAction::CAPTURE_PRESET},
  {"resetParams", AppAction::RESET_PARAMS},
  {"loadScene", AppAction::LOAD_SCENE},
});

template<>
const EnumTypeInfo<AppAction>& ofxTCommon::getEnumInfo() {
  return AppActionType;
}

std::ostream& operator<<(std::ostream& os, const AppAction& action) {
  return os << enumToString(action);
}

AppActionHandler::AppActionHandler() {
  AppSystem::get().appActionTriggered += [&](AppActionEventArgs& event) {
    if (performAction(event.value())) {
      event.markHandled();
    }
  };
}
