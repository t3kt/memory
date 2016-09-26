//
//  ObserverAttributes.h
//  memory
//
//  Created by tekt on 9/25/16.
//
//

#ifndef ObserverAttributes_h
#define ObserverAttributes_h

#include <ofTypes.h>
#include "../core/Component.h"
#include "../core/ObserverEntity.h"
#include "../core/Params.h"
#include "../rendering/Colors.h"
#include "../rules/EntityAttributes.h"

class Context;

class ObserverAttributesParams
: public Params {
  ObserverAttributesParams() {
  }
};

class ObserverAttributes
: public ComponentBase {
public:
  using Params = ObserverAttributesParams;

  ObserverAttributes(Context& context,
                     const Params& params);

  void setup() override;
  void update() override;

private:
  const Params& _params;
  const ColorTheme& _colorTheme;
  Context& _context;

  EntityAttributePtr<ObserverEntity, ofFloatColor> _color;
};

#endif /* ObserverAttributes_h */
