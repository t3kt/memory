//
//  OccurrenceEntity.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef OccurrenceEntity_h
#define OccurrenceEntity_h

#include <ofTypes.h>
#include <map>
#include "Common.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include "Params.h"
#include <iostream>
#include "ValueSupplier.h"

class ObserverEntity;

class OccurrenceEntity
: public ParticleObject {
public:
  class Params : public ::Params {
  public:
    Params();
    
    void initPanel(ofxGuiGroup& panel) override;
    
    RandomValueSupplier<float> radius;
    RandomValueSupplier<ofVec3f> spawnArea;
    ofParameter<ofFloatColor> markerColor;
    ofParameter<ofFloatColor> rangeColor;
    ofParameter<ofFloatColor> connectorColor;
    ofParameter<float> markerSize;
  };
  static shared_ptr<OccurrenceEntity> spawn(const Params& params);
  
  OccurrenceEntity(ofVec3f pos, float radius, const Params& params);
  virtual ~OccurrenceEntity() {}
  
  void addObserver(shared_ptr<ObserverEntity> observer);
  
  void removeObserver(ObjectId id);
  
  bool hasConnectedObservers() const {
    return !_connectedObservers.empty();
  }
  
  float getAmountOfObservation(const State& state) const;
  
  void update(const State& state) override;
  
  void draw(const State& state) override;
  
  void handleDeath() override;
  
  void output(std::ostream& os) const override;
  
  float originalRadius() const { return _originalRadius; };
  
private:
  void recalculateRadius();
  
  const Params& _params;
  const float _originalRadius;
  float _actualRadius;
  std::map<ObjectId, shared_ptr<ObserverEntity>> _connectedObservers;
};

#endif /* OccurrenceEntity_h */
