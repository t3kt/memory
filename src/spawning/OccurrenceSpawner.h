//
//  OccurrenceSpawner.h
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#ifndef OccurrenceSpawner_h
#define OccurrenceSpawner_h

#include "../core/Params.h"
#include "../spawning/Spawner.h"
#include "../core/ValueSupplier.h"

class Bounds;
class Context;
class OccurrenceEntity;
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
    params.add(chain
               .setKey("chain")
               .setName("Chain")
               .setChanceValueAndDefault(0.2));
    params.add(chainCount
               .setKey("chainCount")
               .setName("Chain Count")
               .setParamRanges(0, 20)
               .setParamValuesAndDefaults(1, 5));
    params.add(chainDistance
               .setKey("chainDistance")
               .setName("Chain Distance")
               .setParamRanges(0, 100)
               .setParamValuesAndDefaults(8, 40));
  }

  RandomValueSupplier<float> radius;
  SimpleRandomVectorSupplier initialVelocity;
  RandomBoolSupplier chain;
  RandomValueSupplier<int> chainCount;
  SimpleRandomVectorSupplier chainDistance;
};

class OccurrenceSpawnerCore {
public:
  OccurrenceSpawnerCore(const OccurrencesSpawnerParamsMixin& params,
                        const Bounds& bounds,
                        OccurrencesController& controller)
  : _params(params)
  , _bounds(bounds)
  , _controller(controller) { }

  int spawnEntities(Context& context);
  
private:
  void updateState(Context& context);

  std::shared_ptr<OccurrenceEntity>
  spawnEntity(Context& context,
              float radius,
              const ofVec3f& pos,
              std::shared_ptr<OccurrenceEntity> prev);

  const OccurrencesSpawnerParamsMixin& _params;
  const Bounds& _bounds;
  OccurrencesController& _controller;
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
  RateOccurrenceSpawner(Context& context,
                        const Params& params,
                        const Bounds& bounds,
                        OccurrencesController& controller)
  : RateSpawner(context, params)
  , _core(params,
          bounds,
          controller) { }

protected:
  void spawnEntities(int count) override;

  OccurrenceSpawnerCore _core;
};

#endif /* OccurrenceSpawner_h */
