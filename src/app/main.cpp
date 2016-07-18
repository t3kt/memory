#include "AppSystem.h"

int main() {
  AppSystem::initialize();
  AppSystem::get().main();
}
