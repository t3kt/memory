//
//  ObserversController.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#include <ofLog.h>
#include "AppSystem.h"
#include "ObserversController.h"
#include "SimulationApp.h"

class IntervalObserverSpawner
: public IntervalSpawner {
public:
  IntervalObserverSpawner(ObserversController& controller)
  : IntervalSpawner(controller._params.spawner)
  , _controller(controller) { }
protected:
  void spawnEntities(Context& context) override {
    _controller.spawnRandomObserver();
  }

  ObserversController& _controller;
};

class RateObserverSpawner
: public RateSpawner {
public:
  RateObserverSpawner(ObserversController& controller)
  : RateSpawner(controller._params.rateSpawner)
  , _controller(controller) { }
protected:
  void spawnEntities(Context& context, int count) override {
    for (int i = 0; i < count; ++i) {
      _controller.spawnRandomObserver();
    }
  }

  ObserversController& _controller;
};

ObserversController::ObserversController(const ObserversController::Params& params,
                                         const Bounds& bounds,
                                         Context& context,
                                         SimulationEvents& events)
: _params(params)
, _bounds(bounds)
, _events(events)
, _observers(context.observers)
, _context(context) {
}

void ObserversController::setup(const ColorTheme& colors) {
  _thresholdRenderer = std::make_shared<ThresholdRenderer<ObserverEntity>>(_observers, _params.threshold, colors.getColor(ColorId::OBSERVER_THRESHOLD_CONNECTOR));
  _observerRenderer = std::make_shared<ObserverRenderer>(_params.renderer, colors, _observers);
  _observerConnectorRenderer = std::make_shared<ObserverObserverConnectorRenderer>(_params.connectorRenderer, colors.getColor(ColorId::OBSERVER_CONNECTOR), _observers);
  _spawner = std::make_shared<IntervalObserverSpawner>(*this);
  _rateSpawner = std::make_shared<RateObserverSpawner>(*this);

  registerAsActionHandler();
}

bool ObserversController::performAction(AppAction action) {
  const auto& state = AppSystem::get().simulation()->state();
  switch (action) {
    case AppAction::SPAWN_FEW_OBSERVERS:
      spawnObservers(5);
      break;
    case AppAction::SPAWN_MANY_OBSERVERS:
      spawnObservers(100);
      break;
    case AppAction::KILL_FEW_OBSERVERS:
      killObservers(5);
      break;
    case AppAction::KILL_MANY_OBSERVERS:
      killObservers(100);
      break;
    default:
      return false;
  }
  return true;
}

void ObserversController::update() {
  _observers.performAction([&](std::shared_ptr<ObserverEntity> observer) {
    observer->update(_context.state);
  });

  _observers.cullDeadObjects([&](std::shared_ptr<ObserverEntity> observer) {
    observer->detachConnections();
    ObserverEventArgs e(*observer);
    _events.observerDied.notifyListeners(e);
  });

  _spawner->update(_context);
  _rateSpawner->update(_context);
  _context.state.observerCount = _observers.size();

  _observerRenderer->update(_context.state);
  _thresholdRenderer->update();
}

void ObserversController::draw() {
  _observerRenderer->draw(_context.state);
  _observerConnectorRenderer->draw(_context.state);
  _thresholdRenderer->draw();
}

bool ObserversController::registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence) {
  bool connected = false;
  
  _observers.performAction([&] (std::shared_ptr<ObserverEntity> observer) {
    float dist = occurrence->position().distance(observer->position());
    if (dist <= occurrence->originalRadius()) {
      occurrence->addObserver(observer);
      observer->addOccurrence(occurrence);
      connected = true;
    }
    
  });
  return connected;
}

void ObserversController::spawnRandomObserver() {
  ofVec3f pos = _bounds.randomPoint();
  float life = _params.lifetime.getValue();
  auto observer = std::make_shared<ObserverEntity>(pos,
                                                   life,
                                                   _context.state);
  observer->setVelocity(_params.initialVelocity.getValue());
  _observers.add(observer);
  ObserverEventArgs e(*observer);
  _events.observerSpawned.notifyListeners(e);
}

void ObserversController::spawnObservers(int count) {
  for (int i = 0; i < count; ++i) {
    spawnRandomObserver();
  }
}

void ObserversController::killObservers(int count) {
  int i = 0;
  for (auto& observer : _observers) {
    if (i >= count) {
      return;
    }
    observer->kill();
    i++;
  }
}
