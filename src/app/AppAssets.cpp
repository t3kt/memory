//
//  AppAssets.cpp
//

#include <memory>
#include <ofxAssimpModelLoader.h>
#include "../app/AppAssets.h"
#include "../app/AppSystem.h"
#include "../core/Logging.h"

class AssetData {
public:
  AssetData();

  ofMesh navMarkerMesh;
  ofMesh observerMarkerMesh;
  ofMesh occurrenceMarkerMesh;
  ofShader markerInstanceShader;
  std::vector<ofImage> textureImages;
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

const std::vector<ofImage>& AppAssets::textureImages() {
  return getData().textureImages;
}

static void loadImages(std::string directoryPath,
                       std::vector<ofImage>& images) {
  ofDirectory dir(directoryPath);
  dir.allowExt("jpg");
  dir.listDir();
  for (const auto& file : dir.getFiles()) {
    ofImage image;
    if (!image.load(file.getAbsolutePath())) {
      AppSystem::get().log().app().logWarning("Unable to load texture image: " + file.getAbsolutePath());
      continue;
    }
    AppSystem::get().log().app().logNotice("Loaded texture image: " + file.getAbsolutePath());
    images.push_back(image);
  }
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
  loadImages("textures", textureImages);
}

