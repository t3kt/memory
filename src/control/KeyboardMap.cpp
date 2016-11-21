//
//  KeyboardMap.cpp
//

#include "../app/AppSystem.h"
#include "../app/SimulationApp.h"
#include "../control/CommandsController.h"
#include "../control/KeyboardMap.h"

void KeyboardMap::registerCommand(int key,
                                  KeyboardCommandMapping mapping) {
  _mappings.emplace(key, mapping);
}

bool KeyboardMap::handleKeyPressed(ofKeyEventArgs event) {
  auto iter = _mappings.find(event.key);
  if (iter == _mappings.end()) {
    return false;
  }
  const auto& mapping = iter->second;
  return AppSystem::get().simulation().commands()
      .perform(mapping.name(), mapping.args());
}
