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
#include "../app/AppActions.h"
#include "../gui/AppGui.h"
#include "../app/AppParameters.h"
#include "../core/Common.h"
#include "../control/MidiController.h"
#include "../core/State.h"

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
