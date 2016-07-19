//
//  AppAssets.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef AppAssets_h
#define AppAssets_h

#include <ofMesh.h>
#include <ofShader.h>

class AppAssets {
public:
  static const ofMesh& navMarkerMesh();
  static const ofMesh& occurrenceMarkerMesh();
  static const ofMesh& observerMarkerMesh();
  static const ofShader& markerInstanceShader();
};

#endif /* AppAssets_h */
