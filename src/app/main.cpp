#include <ofMain.h>
//#include "BehaviorApp.h"
#include "MemoryApp.h"

int main( ){
  ofSetupOpenGL(1024,768,OF_WINDOW);

//  ofRunApp(new BehaviorApp());
  ofRunApp(new MemoryApp());
}
