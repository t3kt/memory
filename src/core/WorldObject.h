//
//  WorldObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef WorldObject_h
#define WorldObject_h

#include <functional>
#include <iostream>
#include <memory>
#include "../core/Common.h"
#include "../core/JsonIO.h"
#include "../core/Serialization.h"

typedef int ObjectId;

const ObjectId NO_OBJECT_ID = -1;

enum class EntityType {
  ANIMATION,
  NAVIGATOR,
  OBSERVER,
  OCCURRENCE,
};

class Info;

class WorldObject
: public Outputable
, public Serializable {
public:
  WorldObject();
  virtual ~WorldObject() {}
  
  const ObjectId& id() const { return _id; }

  bool alive() const { return _alive; }

  void kill() { _alive = false; }
  
  virtual bool visible() const { return this->alive() && _alpha > 0; }

  float alpha() const { return _alpha; }
  void setAlpha(float alpha) { _alpha = alpha; }

  const ofVec3f& screenPos() const { return _screenPos; }

  void setScreenPos(const ofVec3f& pos) { _screenPos = pos; }

  virtual EntityType entityType() const = 0;

  virtual void deserializeFields(const Json& obj,
                                 const SerializationContext& context) override;
  virtual std::string typeName() const override;

  virtual void fillInfo(Info& info) const;

  using ObjectPtrRefAction =
  std::function<void(std::shared_ptr<WorldObject>)>;
  virtual void performActionOnConnected(ObjectPtrRefAction action) {}

  virtual const ofVec3f& position() const = 0;

  virtual bool hasConnections() const { return false; }
protected:
  virtual void outputFields(std::ostream& os) const override;
  virtual void addSerializedFields(Json::object& obj,
                                   const SerializationContext& context) const override;
private:
  float _alpha;
  bool _alive;
  ObjectId _id;
  ofVec3f _screenPos;
};

#endif /* WorldObject_h */
