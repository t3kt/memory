//
//  Status.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Status_h
#define Status_h

#include <ofxTextAlignTTF.h>
#include <utility>
#include <vector>
#include "../core/Context.h"
#include "../core/Info.h"

class StatusInfoController {
public:
  StatusInfoController(const Context& context);

  void update();
  void draw();
private:
  const Context& _context;
  ofxTextAlignTTF _text;
  InfoBox _infoBox;
  Info _info;
};

#endif /* Status_h */
