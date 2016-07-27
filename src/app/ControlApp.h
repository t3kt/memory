//
//  ControlApp.h
//  memory
//
//  Created by tekt on 7/10/16.
//
//

#ifndef ControlApp_h
#define ControlApp_h

#include <memory>
#include <ofMain.h>
#include "AppActions.h"
#include "AppGui.h"
#include "AppParameters.h"
#include "Common.h"
#include "MidiController.h"
#include "State.h"

class EventLoggers;

class ControlApp
: public AppActionHandler {
public:
  ControlApp(MemoryAppParameters& appParams)
  : _appParams(appParams) { }

  void setup();
  void update();
  void draw();

  bool performAction(AppAction action) override;
private:
  void updateLogState();

  void loadSettings();
  void saveSettings();

  MemoryAppParameters& _appParams;
  std::shared_ptr<EventLoggers> _eventLoggers;
  std::shared_ptr<AppGui> _gui;
  std::shared_ptr<MidiController> _midi;
};

#endif /* ControlApp_h */
