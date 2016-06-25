//
//  Params.h
//  memory-prototype-2
//
//  Created by tekt on 6/24/16.
//
//

#ifndef Params_h
#define Params_h

#include <string>
#include <ofParameterGroup.h>
#include <ofxGuiGroup.h>

class Params : public ofParameterGroup {
public:
  Params() {}
  explicit Params(std::string label) {
    setName(label);
  }
  
  virtual void initPanel(ofxGuiGroup& panel) {};
};

#endif /* Params_h */
