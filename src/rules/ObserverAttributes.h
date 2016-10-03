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
class ObserverParams;
class ObserverRenderingParams;

class ObserverAttributes
: public ComponentBase
, public NonCopyable {
public:
  using Params = ObserverParams;

  ObserverAttributes(Context& context,
                     const ObserverParams& params,
                     const ObserverRenderingParams& renderParams);

  void setup() override;
  void update() override;

private:
  const ObserverParams& _params;
  const ObserverRenderingParams& _renderParams;
  const ColorTheme& _colorTheme;
  Context& _context;

  EntityAttributePtr<ObserverEntity, ofFloatColor> _color;
};

#endif /* ObserverAttributes_h */
