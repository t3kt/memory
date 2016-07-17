//
//  Navigator.cpp
//  memory
//
//  Created by tekt on 7/16/16.
//
//

#include "Navigator.h"

class PointLocation
: public NavLocation {
public:
  WorldObject* object() override { return nullptr; }
  const WorldObject* object() const override { return nullptr; }
  const ofVec3f& position() const override { return _point; }
  NavLocationType type() const override {
    return NavLocationType::POINT;
  }

  NavStep nextStep(NavContext& context) override;

private:
  ofVec3f _point;
};

NavStep PointLocation::nextStep(NavContext &context) {
  //.....
  return NavStep::none();
}
