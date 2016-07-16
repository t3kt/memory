#include "AppSystem.h"





#include <iostream>
#include <string>

static void TEST_LOOKUP_PATH(const std::string path) {
  auto& params = *AppSystem::get().params();
  TParamBase* param = params.lookupPath(path);
  std::cout << "TEST looking up path '" << path << "': ";
  if (!param) {
    std::cout << "NULL" << std::endl;
  } else {
    std::cout << "found param: (" << param->getKey() << ") " << param->asString() << std::endl;
  }
}

static void TEST_FIND_KEY(Params& params, const std::string key) {
  TParamBase* param = params.findKey(key);
  std::cout << "TEST find key '" << key << "': ";
  if (!param) {
    std::cout << "NULL" << std::endl;
  } else {
    std::cout << "found param: (" << param->getKey() << ") " << param->asString() << std::endl;
  }
}

int main() {
  AppSystem::initialize();

  TEST_FIND_KEY(*AppSystem::get().params(), "core");
  TEST_FIND_KEY(AppSystem::get().params()->core, "camera");

  TEST_LOOKUP_PATH("core");
  TEST_LOOKUP_PATH("core.debug");
  TEST_LOOKUP_PATH("core.debug.showBounds");

  AppSystem::get().main();
}
