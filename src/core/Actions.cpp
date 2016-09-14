//
//  Actions.cpp
//  memory
//
//  Created by tekt on 9/13/16.
//
//

#include "../core/Actions.h"
#include "../core/Context.h"

void ActionsController::addActionAt(ActionPtr action, float time) {
  _actions.push_back(Entry(action, time));
}

void ActionsController::addDelayed(ActionPtr action, float delay) {
  addActionAt(action, _context.time() + delay);
}

void ActionsController::update() {
  auto now = _context.time();
  std::vector<Entry> newEntries;
  for (auto i = _actions.begin();
       i != _actions.end();) {
    auto& entry = *i;
    if (now >= entry.time) {
      auto result = entry.action->run(_context);
      if (result.isReschedule()) {
        newEntries.push_back(entry.withTime(result.rescheduleTime()));
      }
      i = _actions.erase(i);
    } else {
      i++;
    }
  }
  _actions.insert(_actions.end(), newEntries.begin(), newEntries.end());
}
