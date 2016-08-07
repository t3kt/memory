//
//  OccurrenceSpawner.h
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#ifndef OccurrenceSpawner_h
#define OccurrenceSpawner_h

#include "Params.h"
#include "Spawner.h"
#include "ValueSupplier.h"

class Bounds;
class Context;
class OccurrencesController;

class OccurrencesSpawnerParamsMixin {
public:
  void mixInto(Params& params) {
    params.add(radius
               .setKey("radius")
               .setName("Radius Range")
               .setParamValuesAndDefaults(0, 80)
               .setParamRanges(0, 400));
    params.add(initialVelocity
               .setKey("initialVelocity")
               .setName("Initial Velocity")
               .setParamValuesAndDefaults(0, 2)
               .setParamRanges(0, 20));
  }

  RandomValueSupplier<float> radius;
  SimpleRandomVectorSupplier initialVelocity;
};

class RateOccurrenceSpawnerParams
: public RateSpawnerParams
, public OccurrencesSpawnerParamsMixin {
public:
  RateOccurrenceSpawnerParams() {
    OccurrencesSpawnerParamsMixin::mixInto(*this);
  }
};

class RateOccurrenceSpawner
: public RateSpawner<RateOccurrenceSpawnerParams> {
public:
  RateOccurrenceSpawner(const Params& params,
                        const Bounds& bounds,
                        OccurrencesController& controller)
  : RateSpawner(params)
  , _bounds(bounds)
  , _controller(controller) { }

protected:
  void spawnEntities(Context& context, int count) override;

  const Bounds& _bounds;
  OccurrencesController& _controller;
};

class IntervalOccurrenceSpawnerParams
: public IntervalSpawnerParams
, public OccurrencesSpawnerParamsMixin {
public:
  IntervalOccurrenceSpawnerParams() {
    OccurrencesSpawnerParamsMixin::mixInto(*this);
  }
};

class IntervalOccurrenceSpawner
: public IntervalSpawner<IntervalOccurrenceSpawnerParams> {
public:
  IntervalOccurrenceSpawner(const Params& params,
                            const Bounds& bounds,
                            OccurrencesController& controller)
  : IntervalSpawner(params)
  , _bounds(bounds)
  , _controller(controller) { }
protected:
  void spawnEntities(Context& context) override;

  const Bounds& _bounds;
  OccurrencesController& _controller;
};

#endif /* OccurrenceSpawner_h */
