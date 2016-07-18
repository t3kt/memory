//
//  ObserverRenderer.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef ObserverRenderer_h
#define ObserverRenderer_h

#include "EntityRenderer.h"

class ObserverRenderer
: public EntityRenderer<ObserverEntity> {
public:

  ObserverRenderer(const Params& params,
                   const ColorTheme& colors,
                   ObjectManager<ObserverEntity>& entities);
protected:
  ObjectManager<ObserverEntity>::StorageList::iterator begin() override {
    return _entities.begin();
  }
  ObjectManager<ObserverEntity>::StorageList::iterator end() override {
    return _entities.end();
  }
  void drawEntity(const ObserverEntity& entity, const ofFloatColor& baseColor, float size, const State& state) override;
private:
  ObjectManager<ObserverEntity>& _entities;
};

#endif /* ObserverRenderer_h */
