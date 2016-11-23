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
