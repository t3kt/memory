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
#include "../core/Common.h"
#include "../core/Component.h"
#include "../core/Context.h"
#include "../core/Info.h"

class StatusInfoController
: public NonCopyable
, public ComponentBase {
public:
  StatusInfoController(const Context& context);

  void update() override;
  void draw() override;
private:
  const Context& _context;
  ofxTextAlignTTF _text;
  InfoBox _infoBox;
  Info _info;
};

#endif /* Status_h */
