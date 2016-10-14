//
//  AppAssets.h
//

#ifndef AppAssets_h
#define AppAssets_h

#include <ofImage.h>
#include <ofMesh.h>
#include <ofShader.h>
#include <vector>

// Assets loaded from external files.
class AppAssets {
public:
  static const ofMesh& navMarkerMesh();
  static const ofMesh& occurrenceMarkerMesh();
  static const ofMesh& observerMarkerMesh();
  static const ofShader& markerInstanceShader();
  static const std::vector<ofImage>& textureImages();
};

#endif /* AppAssets_h */
