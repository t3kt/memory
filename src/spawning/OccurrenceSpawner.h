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
#include "../control/Params.h"
#include "../control/ValueSupplier.h"
#include "../spawning/Spawner.h"

class BoundsController;
class Context;
class OccurrenceEntity;
class OccurrencesController;
class SimulationEvents;

class RateOccurrenceSpawnerParams
: public RateSpawnerParams {
public:
  RateOccurrenceSpawnerParams() {
    add(radius
        .setKey("radius")
        .setName("Radius Range")
        .setParamValuesAndDefaults(0, 80)
        .setParamRanges(0, 400));
    add(initialVelocity
        .setKey("initialVelocity")
        .setName("Initial Velocity")
        .setParamValuesAndDefaults(0, 2)
        .setParamRanges(0, 20));
    add(chain
        .setKey("chain")
        .setName("Chain")
        .setChanceValueAndDefault(0.2));
    add(chainCount
        .setKey("chainCount")
        .setName("Chain Count")
        .setParamRanges(0, 20)
        .setParamValuesAndDefaults(1, 5));
    add(chainDistance
        .setKey("chainDistance")
        .setName("Chain Distance")
        .setParamRanges(0, 100)
        .setParamValuesAndDefaults(8, 40));
    add(sequence
        .setKey("sequence")
        .setName("Sequence")
        .setChanceValueAndDefault(0.1));
    add(sequenceCount
        .setKey("sequenceCount")
        .setName("Sequence Count")
        .setParamRanges(2, 30)
        .setParamValuesAndDefaults(2, 10));
    add(sequenceInterval
        .setKey("sequenceInterval")
        .setName("Sequence Interval")
        .setParamRanges(0, 60)
        .setParamValuesAndDefaults(5, 20));
    add(sequenceStepDistance
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

class OccurrenceSpawner
: public RateSpawner<RateOccurrenceSpawnerParams>
, public AppActionHandler {
public:
  OccurrenceSpawner(Context& context,
                    const Params& params,
                    const BoundsController& bounds);

  std::shared_ptr<OccurrenceEntity>
  spawnSequenceStepEntity(std::shared_ptr<OccurrenceEntity> prev);

  bool performAction(AppAction action) override;

protected:
  void spawnEntities(int count) override;

private:
  int spawnEntityGroup();

  std::shared_ptr<OccurrenceEntity>
  spawnEntity(float radius,
              const ofVec3f& pos,
              std::shared_ptr<OccurrenceEntity> prev);

  bool spawnNewSequence();

  bool tryAddEntity(std::shared_ptr<OccurrenceEntity> occurrence);

  const Params& _params;
  const BoundsController& _bounds;
  SimulationEvents& _events;

  friend class OccurrenceSequenceSpawnAction;
};

#endif /* OccurrenceSpawner_h */
