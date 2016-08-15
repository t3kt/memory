//
//  AppAssets.cpp
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#include <memory>
#include <ofxAssimpModelLoader.h>
#include "../app/AppAssets.h"

class AssetData {
public:
  AssetData();

  ofMesh navMarkerMesh;
  ofMesh observerMarkerMesh;
  ofMesh occurrenceMarkerMesh;
  ofShader markerInstanceShader;
};

std::shared_ptr<AssetData> assetData;

const AssetData& getData() {
  if (!assetData) {
    assetData = std::make_shared<AssetData>();
  }
  return *assetData;
}

const ofMesh& AppAssets::navMarkerMesh() {
  return getData().navMarkerMesh;
}

const ofMesh& AppAssets::observerMarkerMesh() {
  return getData().observerMarkerMesh;
}

const ofMesh& AppAssets::occurrenceMarkerMesh() {
  return getData().occurrenceMarkerMesh;
}

const ofShader& AppAssets::markerInstanceShader() {
  return getData().markerInstanceShader;
}

AssetData::AssetData() {
  ofxAssimpModelLoader loader;
  loader.loadModel("nav-marker.stl", false);
  navMarkerMesh = loader.getMesh(0);
  loader.loadModel("observer-marker.stl", false);
  observerMarkerMesh = loader.getMesh(0);
  loader.loadModel("occurrence-marker.stl", false);
  occurrenceMarkerMesh = loader.getMesh(0);
//  markerInstanceShader.load("marker-shader.vert", "marker-shader-frag");
}
