//
//  EventLogging.h
//  memory
//
//  Created by tekt on 8/27/16.
//
//

#ifndef EventLogging_h
#define EventLogging_h

#include <functional>
#include "../core/SimulationEvents.h"

class EventLoggers {
public:
  EventLoggers();

  void attach(SimulationEvents& events);
  void detach(SimulationEvents& events);
private:
  std::function<void(AnimationEventArgs&)> _animationSpawned;
  std::function<void(AnimationEventArgs&)> _animationDied;
  std::function<void(ObserverEventArgs&)> _observerSpawned;
  std::function<void(ObserverEventArgs&)> _observerDied;
  std::function<void(OccurrenceEventArgs&)> _occurrenceSpawned;
  std::function<void(OccurrenceEventArgs&)> _occurrenceSpawnFailed;
  std::function<void(OccurrenceEventArgs&)> _occurrenceDied;
  std::function<void(NavigatorEventArgs&)> _navigatorSpawned;
  std::function<void(NavigatorEventArgs&)> _navigatorReachedLocation;
  std::function<void(NavigatorEventArgs&)> _navigatorDied;
};

#endif /* EventLogging_h */
