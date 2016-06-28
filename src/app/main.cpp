#include <ofMain.h>
#include "MemoryApp.h"

int main( ){
  ofSetupOpenGL(1024,768,OF_WINDOW);

  ofRunApp(new MemoryApp());
}
