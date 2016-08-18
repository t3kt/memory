//
//  AnalysisController.h
//  memory
//
//  Created by tekt on 8/17/16.
//
//

#ifndef AnalysisController_h
#define AnalysisController_h

#include "../analysis/SimulationStats.h"

class Context;

class AnalysisController {
public:
  AnalysisController(Context& context)
  : _context(context) { }

  void update();

private:
  Context& _context;
};

#endif /* AnalysisController_h */
