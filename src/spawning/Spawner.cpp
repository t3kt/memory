//
//  Spawner.cpp
//

#include "../app/AppSystem.h"
#include "../spawning/Spawner.h"

class SpawnCommand
: public Command {
public:
  SpawnCommand(std::string name,
               std::string label,
               std::shared_ptr<Spawner> spawner)
  : Command(name, label)
  , _spawner(spawner) { }

  bool perform(const CommandArgs& args) override {
    int count = 1;
    if (args.hasArgType<int>(0)) {
      count = args.get<int>(0);
    }
    _spawner->spawnNow(count);
    return true;
  }

private:
  std::shared_ptr<Spawner> _spawner;
};

CommandRegistration
Spawner::registerSpawnCommand(std::string name,
                              std::string label) {
  auto command = std::make_shared<SpawnCommand>(name, label, shared_from_this());
  return AppSystem::get().commands().registerCommand(command);
}
