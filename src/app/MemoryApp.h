//
//  MemoryApp.h
//  memory-prototype-2
//
//  Created by tekt on 6/21/16.
//
//

#ifndef MemoryApp_h
#define MemoryApp_h

#include <ofMain.h>
#include <ofxGui.h>
#include <ofxScreenLoggerChannel.h>
#include <ofxMultiLoggerChannel.h>
#include <ofxPostProcessing.h>
#include "State.h"
#include "AppParameters.h"
#include "ObserversController.h"
#include "OccurrencesController.h"
#include "AnimationsController.h"
#include "Status.h"
#include "AppGui.h"
#include "Clock.h"

class FPSInfoProvider;
class GuiPanel;

class MemoryApp : public ofBaseApp {
public:
  void setup() override;
  void update() override;
  void draw() override;

  void keyPressed(int key) override;
private:
  State _state;
  MemoryAppParameters _appParams;
  shared_ptr<ObserversController> _observers;
  shared_ptr<OccurrencesController> _occurrences;
  shared_ptr<AnimationsController> _animations;
  shared_ptr<StatusInfoController> _statusController;
  ofEasyCam _cam;
  shared_ptr<GuiPanel> _gui;
  shared_ptr<AppGui> _NEW_gui;
  shared_ptr<ofxScreenLoggerChannel> _screenLoggerChannel;
  shared_ptr<ofxMultiLoggerChannel> _multiLoggerChannel;
  shared_ptr<FPSInfoProvider> _fpsProvider;
  shared_ptr<Clock> _clock;
  ofxPostProcessing _postProc;
};

#endif /* MemoryApp_h */
