//
//  ThresholdRenderer.h
//  memory-prototype-2
//
//  Created by tekt on 6/27/16.
//
//

#ifndef ThresholdRenderer_h
#define ThresholdRenderer_h

#include "Params.h"
#include "State.h"
#include "ObjectManager.h"
#include <ofColor.h>
#include <ofMesh.h>

class ThreshData {
public:
  ThreshData();
  void initialize(float minDist, float maxDist, ofFloatColor color);
  bool tryAdd(ofVec3f pos1, ofVec3f pos2);
private:
  float _minDist;
  float _maxDist;
  ofFloatColor _color;
  ofMesh _mesh;

  friend class AbstractThresholdRenderer;
};

class AbstractThresholdRenderer {
public:
  class Params : public ::Params {
  public:
    Params(std::string);

    ofParameter<bool> enabled;
    ofParameter<ofFloatColor> color;
    FloatValueRange range;
  };

  AbstractThresholdRenderer(const Params& params)
  : _params(params) {}

  void update(const State& state);
  void draw(const State& state);
protected:
  virtual void populateThreshData(ThreshData* data) = 0;
private:
  const Params& _params;
  ThreshData _data;
};

template<typename T>
class ThresholdRenderer
: public AbstractThresholdRenderer {
public:
  ThresholdRenderer(ObjectManager<T>& objects, const Params& params)
  : AbstractThresholdRenderer(params)
  , _objects(objects) { }
protected:
  void populateThreshData(ThreshData* data) override {
    auto end = _objects.end();
    for (auto i = _objects.begin();
         i != end;
         i++) {
      for (auto j = i;
           j != end;
           j++) {
        data->tryAdd((*i)->position(), (*j)->position());
      }
    }
  }
private:
  ObjectManager<T>& _objects;
};

#endif /* ThresholdRenderer_h */