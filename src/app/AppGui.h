//
//  AppGui.h
//

#ifndef AppGui_h
#define AppGui_h

#include <memory>
#include "../app/AppActions.h"
#include "../app/AppParameters.h"

class AppGuiImpl;

// Control panel GUI which modifies Params
class AppGui {
public:
  AppGui(MemoryAppParameters& appParams,
         AppActionHandler& actionHandler);

  void draw();
private:
  std::shared_ptr<AppGuiImpl> _impl;
};

#endif /* AppGui_h */
