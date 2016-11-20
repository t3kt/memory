//
//  OscController.h
//

#pragma once

#include <memory>
#include <ofxOsc.h>
#include <unordered_map>
#include "../control/Params.h"
#include "../core/Common.h"
#include "../core/Component.h"

class AbstractOscBinding;
class CommandsController;
class MemoryAppParameters;

class OscController
: public NonCopyable
, public ComponentBase {
public:
  using BindingMap = std::unordered_map<std::string, std::shared_ptr<AbstractOscBinding>>;

  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(inputEnabled
          .setKey("inputEnabled")
          .setName("Input Enabled"));
      add(inputPort
          .setKey("inputPort")
          .setName("Input Port")
          .setValueAndDefault(9999)
          .setRange(0, 65535));
      add(outputEnabled
          .setKey("outputEnabled")
          .setName("Output Enabled"));
      add(outputHost
          .setKey("outputHost")
          .setName("Output Host")
          .setValueAndDefault("localhost"));
      add(outputPort
          .setKey("outputPort")
          .setName("Output Port")
          .setValueAndDefault(9000)
          .setRange(0, 65535));
      add(paramPrefix
          .setKey("paramPrefix")
          .setName("Param Prefix")
          .setValueAndDefault("/param/"));
      add(commandPrefix
          .setKey("commandPrefix")
          .setName("Command Prefix")
          .setValueAndDefault("/cmd/"));
    }
    TParam<std::string> outputHost;
    TParam<int> outputPort;
    TParam<int> inputPort;
    TParam<bool> inputEnabled;
    TParam<bool> outputEnabled;
    TParam<std::string> paramPrefix;
    TParam<std::string> commandPrefix;
  };

  OscController(MemoryAppParameters& appParams,
                CommandsController& commands);

  ~OscController();

  void update() override;

private:
  void handleOpen();
  void handleClose(bool updateParams);
  void loadBindings(::Params& params, const std::string& basePath);
  void handleMessage(const ofxOscMessage& message);
  bool handleMessageAsCommand(const ofxOscMessage& message);
//  void queueOutputMessage(const ofxOscMessage& message);
  void sendMessage(ofxOscMessage message);
  void sendAllParameters();
  void sendParameterConfigs();

  Params& _params;
  MemoryAppParameters& _appParams;
  CommandsController& _commands;
  std::shared_ptr<ofxOscSender> _sender;
  std::shared_ptr<ofxOscReceiver> _receiver;
  BindingMap _bindings;
//  ofxOscBundle _outputBundle;
  bool _receiving;

  friend class AbstractOscBinding;
};
