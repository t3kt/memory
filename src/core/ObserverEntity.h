//
//  ObserverEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef ObserverEntity_h
#define ObserverEntity_h

#include <ofTypes.h>
#include "Params.h"
#include "Common.h"
#include "WorldObject.h"
#include <vector>
#include <iostream>
#include "ValueSupplier.h"

class OccurrenceEntity;

class ObserverEntity
: public WorldObject {
public:
  class Params : public ::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    RandomValueSupplier<float> lifetime;
    RandomValueSupplier<ofVec3f> spawnArea;
    ofParameter<ofFloatColor> color;
    ofParameter<float> drawRadius;
  };
  
  static shared_ptr<ObserverEntity> spawn(const Params& params, const State& state);
  
  ObserverEntity(ofVec3f pos, float life, const Params& params, const State& state);
  virtual ~ObserverEntity() override {}
  
  void addOccurrence(shared_ptr<OccurrenceEntity> occurrence);
  
  std::vector<shared_ptr<OccurrenceEntity>>& getConnectedOccurrences() {
    return _connectedOccurrences;
  }
  
  float getRemainingLifetimeFraction(const State& state) const;
  
  void draw(const State& state) override;
  
  void output(std::ostream& os) const override;
  
  bool isAlive(const State& state) const override {
    return getRemainingLifetimeFraction(state) > 0;
  }
  
  void handleDeath() override;
  
  float lifetime() const { return _totalLifetime; };
  
private:
  const Params& _params;
  const float _startTime;
  const float _totalLifetime;
  std::vector<shared_ptr<OccurrenceEntity>> _connectedOccurrences;
};

#endif /* ObserverEntity_h */
