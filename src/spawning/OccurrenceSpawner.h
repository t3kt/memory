//
//  OccurrenceSpawner.h
//  memory
//
//  Created by tekt on 8/6/16.
//
//

#ifndef OccurrenceSpawner_h
#define OccurrenceSpawner_h

#include "../app/AppActions.h"
#include "../core/Params.h"
#include "../core/ValueSupplier.h"
#include "../spawning/Spawner.h"

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
    params.add(sequence
               .setKey("sequence")
               .setName("Sequence")
               .setChanceValueAndDefault(0.1));
    params.add(sequenceCount
               .setKey("sequenceCount")
               .setName("Sequence Count")
               .setParamRanges(2, 30)
               .setParamValuesAndDefaults(2, 10));
    params.add(sequenceInterval
               .setKey("sequenceInterval")
               .setName("Sequence Interval")
               .setParamRanges(0, 60)
               .setParamValuesAndDefaults(5, 20));
    params.add(sequenceStepDistance
               .setKey("sequenceStepDistance")
               .setName("Seq Step Distance")
               .setParamRanges(0, 100)
               .setParamValuesAndDefaults(10, 50));
  }

  RandomValueSupplier<float> radius;
  SimpleRandomVectorSupplier initialVelocity;
  RandomBoolSupplier chain;
  RandomValueSupplier<int> chainCount;
  SimpleRandomVectorSupplier chainDistance;
  RandomBoolSupplier sequence;
  RandomValueSupplier<int> sequenceCount;
  RandomValueSupplier<float> sequenceInterval;
  RandomValueSupplier<float> sequenceStepDistance;
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

  std::shared_ptr<OccurrenceEntity>
  spawnSequenceStepEntity(Context& context,
                          std::shared_ptr<OccurrenceEntity> prev);

  const OccurrencesSpawnerParamsMixin& params() const {
    return _params;
  }
  
private:
  void updateState(Context& context);

  std::shared_ptr<OccurrenceEntity>
  spawnEntity(Context& context,
              float radius,
              const ofVec3f& pos,
              std::shared_ptr<OccurrenceEntity> prev);

  bool spawnNewSequence(Context& context);

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
: public RateSpawner<RateOccurrenceSpawnerParams>
, public AppActionHandler {
public:
  RateOccurrenceSpawner(Context& context,
                        const Params& params,
                        const Bounds& bounds,
                        OccurrencesController& controller)
  : RateSpawner(context, params)
  , _core(params,
          bounds,
          controller) {
    registerAsActionHandler();
  }

  bool performAction(AppAction action) override;

protected:
  void spawnEntities(int count) override;

  OccurrenceSpawnerCore _core;
};

#endif /* OccurrenceSpawner_h */
