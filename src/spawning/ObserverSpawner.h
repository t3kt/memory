//
//  ObserverSpawner.h
//

#pragma once

#include "../control/Params.h"
#include "../control/ValueSupplier.h"
#include "../spawning/Spawner.h"

class BoundsController;
class Context;
class SimulationEvents;

class ObserverSpawnerParamsMixin {
public:
  void mixInto(Params& params) {
    params.add(decayRate
               .setKey("decayRate")
               .setName("Decay Rate Range")
               .setParamValuesAndDefaults(0.005, 0.090)
               .setParamRanges(0, 0.5));
    params.add(initialVelocity
               .setKey("initialVelocity")
               .setName("Initial Velocity")
               .setParamValuesAndDefaults(0, 4)
               .setParamRanges(0, 20));
  }

  RandomValueSupplier<float> decayRate;
  SimpleRandomVectorSupplier initialVelocity;
};

class RateObserverSpawnerParams
: public RateSpawnerParams
, public ObserverSpawnerParamsMixin {
public:
  RateObserverSpawnerParams() {
    ObserverSpawnerParamsMixin::mixInto(*this);
  }
};

class RateObserverSpawner
: public RateSpawner<RateObserverSpawnerParams> {
public:
  RateObserverSpawner(Context& context,
                      const Params& params,
                      const BoundsController& bounds);

  void setup() override;

protected:
  void spawnEntities(int count) override;

  void addEntity(std::shared_ptr<ObserverEntity> entity);

  const BoundsController& _bounds;
  SimulationEvents& _events;
};

