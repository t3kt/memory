//
//  ParametersController.h
//  memory
//
//  Created by tekt on 10/12/16.
//
//

#ifndef ParametersController_h
#define ParametersController_h

#include "../app/AppActions.h"
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/JsonIO.h"

class MemoryAppParameters;

class ParametersState
: public NonCopyable
, public JsonReadable
, public JsonWritable {
public:
  ParametersState(MemoryAppParameters& params)
  : _params(params) { }

  Json to_json() const override;
  void read_json(const Json& obj) override;
private:
  MemoryAppParameters& _params;
};

class ParametersController
: public NonCopyable
, public ComponentBase
, public AppActionHandler {
public:
  using State = ParametersState;

  ParametersController(MemoryAppParameters& params)
  : _params(params)
  , _state(params) { }

  void setup() override;
  void update() override;

  bool performAction(AppAction action) override;
  void load();
  void save();
private:
  MemoryAppParameters& _params;
  ParametersState _state;
};

#endif /* ParametersController_h */
