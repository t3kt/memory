//
//  AppAssets.h
//

#ifndef AppAssets_h
#define AppAssets_h

#include <ofMesh.h>
#include <ofShader.h>

// Assets loaded from external files.
class AppAssets {
public:
  static const ofMesh& navMarkerMesh();
  static const ofMesh& occurrenceMarkerMesh();
  static const ofMesh& observerMarkerMesh();
  static const ofShader& markerInstanceShader();
};

#endif /* AppAssets_h */
