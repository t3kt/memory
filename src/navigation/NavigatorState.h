//
//  NavigatorState.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef NavigatorState_h
#define NavigatorState_h

#include <iostream>
#include <memory>
#include "Common.h"
#include "Context.h"
#include "ObserverEntity.h"
#include "WorldObject.h"

class NavigatorState;
using NavigatorStatePtr = std::shared_ptr<NavigatorState>;

class NavigatorState
: public Outputable {
public:
  virtual const ofVec3f& position() const = 0;
  virtual NavigatorStatePtr nextState(Context& context) = 0;
  virtual bool alive() const = 0;

  void output(std::ostream& os) const {
    os << typeName() << "{";
    outputFields(os);
    os << "}";
  }

  virtual std::string typeName() const = 0;

  virtual void fillInfo(Info& info, const std::string& prefix) const = 0;
protected:
  virtual void outputFields(std::ostream& os) const {}
};

class PointNavState
: public NavigatorState {
public:
  PointNavState(ofVec3f point)
  : _point(point) {}

  const ofVec3f& position() const override { return _point; }

  NavigatorStatePtr nextState(Context& context) override;

  bool alive() const override { return true; }

  void fillInfo(Info& info, const std::string& prefix) const override;

  std::string typeName() const override { return "PointNavState"; }
protected:
  void outputFields(std::ostream& os) const override;

private:
  ofVec3f _point;
};

class ObserverNavState
: public NavigatorState {
public:
  ObserverNavState(std::shared_ptr<ObserverEntity> entity)
  : _entity(entity) { }

  const ofVec3f& position() const override {
    return _entity->position();
  }

  ObserverEntity& entity() { return *_entity; }

  NavigatorStatePtr nextState(Context& context) override;

  bool alive() const override { return _entity->alive(); }

  void fillInfo(Info& info, const std::string& prefix) const override;

  std::string typeName() const override { return "ObserverNavState"; }
protected:
  void outputFields(std::ostream& os) const override;

private:
  std::shared_ptr<ObserverEntity> _entity;
};

#endif /* NavigatorState_h */
