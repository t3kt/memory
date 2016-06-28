//
//  ParamsGui.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef ParamsGui_h
#define ParamsGui_h

#include "Params.h"
#include <ofxDatGui.h>

class AbstractParamsGui
: public ParamsGui {
public:
  AbstractParamsGui(Params& params);

  ofxDatGuiFolder* getGuiFolder() override { return _folder; }

  virtual void setup() override;
protected:
  virtual void addControls() = 0;

  ofxDatGuiToggle* addToggle(ofParameter<bool>& param);

  virtual void onToggleEvent(ofxDatGuiToggleEvent e);

private:
  Params& _baseParams;
  ofxDatGuiFolder* _folder;
};

#endif /* ParamsGui_h */
