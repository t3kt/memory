//
//  AppGui.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "AppGui.h"

AppGui::AppGui(MemoryAppParameters& appParams)
: _appParams(appParams) { }

void AppGui::setup() {
//  ofxDatGuiLog::quiet(); // ?

  // TODO: change this to top left after removing old GUI?
  _gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  _gui->addHeader("--{Memory}--");
  _gui->addFRM();

  add(_appParams.debug);
  add(_appParams.bounds);
  add(_appParams.animations);
  add(_appParams.observers);
  add(_appParams.occurrences);
  
  _gui->addFooter();
}

void AppGui::add(Params& params) {
  ParamsGui* paramsGui = params.getGui();
  if (paramsGui != nullptr) {
    _gui->addFolder(paramsGui->getGuiFolder());
  }
}

void AppGui::update() {
  //...
//  _gui->update();
}

void AppGui::draw() {
  //...
//  _gui->draw();
}
