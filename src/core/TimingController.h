//
//  TimingController.h
//  memory
//
//  Created by tekt on 10/10/16.
//
//

#ifndef TimingController_h
#define TimingController_h

#include "../app/AppActions.h"
#include "../core/Clock.h"
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/Params.h"

class Context;

class TimingParams
: public Params {
public:
  TimingParams() {
    add(root
        .setKey("root")
        .setName("Root Clock"));
    root.add(entity
             .setKey("entity")
             .setName("Entity Clock"));
    root.add(physics
             .setKey("physics")
             .setName("Physics Clock"));
  }

  ClockParams root;
  ClockParams entity;
  ClockParams physics;
};

class TimingController
: public ComponentBase
, public NonCopyable
, public AppActionHandler {
public:
  using Params = TimingParams;

  TimingController(Params& params,
                   Context& context)
  : _params(params)
  , _context(context) { }

  void setup() override;
  void update() override;

  bool performAction(AppAction action) override;

private:
  Params& _params;
  Context& _context;
  std::shared_ptr<ClockNode> _rootClock;
  ComponentCollection<ClockNode> _clocks;
};

#endif /* TimingController_h */
