//
//  OscController.cpp
//  memory
//
//  Created by tekt on 7/22/16.
//
//

#include <stdexcept>
#include "../app/AppParameters.h"
#include "../app/AppSystem.h"
#include "../control/OscController.h"

class AbstractOscBinding {
public:
  AbstractOscBinding(std::string path, OscController& controller)
  : _path(path)
  , _controller(controller) { }

  virtual ~AbstractOscBinding() {}

  const std::string& path() const { return _path; }

  virtual void handleMessage(const ofxOscMessage& message) = 0;

  virtual void sendValue() = 0;

  virtual void sendConfig() = 0;
protected:
//  inline void queueMessage(ofxOscMessage& message) {
//    message.setRemoteEndpoint(_controller._params.outputHost.get(),
//                              _controller._params.outputPort.get());
//    _controller.queueOutputMessage(message);
//  }
  inline void sendMessage(const ofxOscMessage& message) {
    _controller.sendMessage(message);
  }

  const std::string _path;
  OscController& _controller;
};

template<typename T>
void addOscMessageArg(ofxOscMessage& message,
                      const T& value);

template<>
void addOscMessageArg(ofxOscMessage& message,
                      const float& value) {
  message.addFloatArg(value);
}

template<>
void addOscMessageArg(ofxOscMessage& message,
                      const int& value) {
  message.addIntArg(value);
}

template<>
void addOscMessageArg(ofxOscMessage& message,
                      const bool& value) {
  message.addBoolArg(value);
}

template<>
void addOscMessageArg(ofxOscMessage& message,
                      const ofFloatColor& value) {
  message.addFloatArg(value.r);
  message.addFloatArg(value.g);
  message.addFloatArg(value.b);
  message.addFloatArg(value.a);
}

template<>
void addOscMessageArg(ofxOscMessage& message,
                      const ofVec3f& value) {
  message.addFloatArg(value.x);
  message.addFloatArg(value.y);
  message.addFloatArg(value.z);
}

template<typename T>
T getOscMessageArg(const ofxOscMessage& message, int i);

template<>
float getOscMessageArg(const ofxOscMessage& message, int i) {
  return message.getArgAsFloat(i);
}

template<>
int getOscMessageArg(const ofxOscMessage& message, int i) {
  return message.getArgAsInt(i);
}

template<>
bool getOscMessageArg(const ofxOscMessage& message, int i) {
  return message.getArgAsBool(i);
}

template<typename T>
T getOscMessageValue(const ofxOscMessage& message) {
  return getOscMessageArg<T>(message, 0);
}

static void logInvalidSize(const ofxOscMessage& message) {
  AppSystem::get().log().control().logWarning([&](ofLog& log) {
    log << "Invalid message size " << message.getNumArgs() << " for path '" << message.getAddress() << "'";
  });
}

template<>
ofFloatColor getOscMessageValue(const ofxOscMessage& message) {
  auto size = message.getNumArgs();
  if (size == 3) {
    return ofFloatColor(message.getArgAsFloat(0),
                        message.getArgAsFloat(1),
                        message.getArgAsFloat(2),
                        1);
  } else if (size == 4) {
    return ofFloatColor(message.getArgAsFloat(0),
                        message.getArgAsFloat(1),
                        message.getArgAsFloat(2),
                        message.getArgAsFloat(3));
  } else {
    logInvalidSize(message);
    return ofFloatColor(0, 0, 0, 1);
  }
}

template<>
ofVec3f getOscMessageValue(const ofxOscMessage& message) {
  auto size = message.getNumArgs();
  if (size == 3) {
    return ofVec3f(message.getArgAsFloat(0),
                   message.getArgAsFloat(1),
                   message.getArgAsFloat(2));
  } else {
    logInvalidSize(message);
    return ofVec3f(0);
  }
}

template<typename T>
Json getParamConfigJson(const TParam<T>& param) {
  return Json::object {
    {"key", param.getKey()},
    {"name", param.getName()},
    {"default", JsonUtil::toJson(param.getDefaultValue())},
  };
}

template<>
Json getParamConfigJson(const TParam<float>& param) {
  return Json::object {
    {"key", param.getKey()},
    {"name", param.getName()},
    {"min", JsonUtil::toJson(param.getMin())},
    {"max", JsonUtil::toJson(param.getMax())},
    {"default", JsonUtil::toJson(param.getDefaultValue())},
  };
}

template<>
Json getParamConfigJson(const TParam<int>& param) {
  return Json::object {
    {"key", param.getKey()},
    {"name", param.getName()},
    {"min", JsonUtil::toJson(param.getMin())},
    {"max", JsonUtil::toJson(param.getMax())},
    {"default", JsonUtil::toJson(param.getDefaultValue())},
  };
}

template<>
Json getParamConfigJson(const TParam<ofVec3f>& param) {
  return Json::object {
    {"key", param.getKey()},
    {"name", param.getName()},
    {"min", JsonUtil::toJson(param.getMin())},
    {"max", JsonUtil::toJson(param.getMax())},
    {"default", JsonUtil::toJson(param.getDefaultValue())},
  };
}

template<typename T>
class OscBinding
: public AbstractOscBinding {
public:
  OscBinding(TParam<T>& param,
             std::string path,
             OscController& controller)
  : AbstractOscBinding(path, controller)
  , _param(param) {
    _param.changed.addListener([this](T& value) {
      onParamChanged(value);
    }, this);
  }

  ~OscBinding() override {
    _param.changed.removeListeners(this);
  }

  void handleMessage(const ofxOscMessage& message) override {
    auto value = getOscMessageValue<T>(message);
    _param.set(value);
  }

  void sendValue() override {
    onParamChanged(_param.get());
  }

  void sendConfig() override {
    ofxOscMessage message;
    message.setAddress("/config");
    auto config = getParamConfigJson(_param);
    message.addStringArg(_path);
    message.addStringArg(config.dump());
    sendMessage(message);
  }

protected:
  void onParamChanged(const T& value) {
    ofxOscMessage message;
    message.setAddress(_path);
    addOscMessageArg(message, value);
    sendMessage(message);
  }

  TParam<T>& _param;
};

template<typename T>
static std::shared_ptr<AbstractOscBinding>
createTypedBinding(TParamBase& param,
                   const std::string& path,
                   OscController& controller) {
  TParam<T>& typedParam = dynamic_cast<TParam<T>&>(param);
  auto binding = new OscBinding<T>(typedParam,
                                   path,
                                   controller);
  return std::shared_ptr<AbstractOscBinding>(binding);
}

static std::shared_ptr<AbstractOscBinding>
createBinding(TParamBase& param,
              const std::string& basePath,
              OscController& controller) {
  auto path = basePath + param.getKey();
  if (param.isGroup()) {
    throw std::invalid_argument("Cannot bind to group " + path);
  }
  const auto& type = param.getTypeInfo();
  if (type == typeid(float)) {
    return createTypedBinding<float>(param, path, controller);
  } else if (type == typeid(int)) {
    return createTypedBinding<int>(param, path, controller);
  } else if (type == typeid(bool)) {
    return createTypedBinding<bool>(param, path, controller);
  } else {
    throw std::invalid_argument("Cannot bind to param " + path
                                + " of type " + type.name());
  }
}

OscController::OscController(MemoryAppParameters& appParams)
: _params(appParams.core.osc)
, _appParams(appParams) {
  TEvent<bool>::VoidFunctor handler = [this]() {
    handleOpen(); // closes if disabled
  };
  _params.enabled.changed.addVoidListener(handler, this);
  _params.inputEnabled.changed.addVoidListener(handler, this);
  _params.outputEnabled.changed.addVoidListener(handler, this);
  handleOpen();
}

OscController::~OscController() {
  handleClose(false);
  _params.enabled.changed.removeListeners(this);
  _params.inputEnabled.changed.removeListeners(this);
  _params.outputEnabled.changed.removeListeners(this);
}

void OscController::handleOpen() {
  handleClose(false);
  if (!_params.enabled.get()) {
    return;
  }
  if (_params.inputEnabled.get()) {
    _receiver = std::make_shared<ofxOscReceiver>();
    _receiver->setup(_params.inputPort.get());
  }
  if (_params.outputEnabled.get()) {
    std::string host = _params.outputHost.get();
    if (host.empty()) {
      host = _params.outputHost.getDefaultValue();
    }
    _sender = std::make_shared<ofxOscSender>();
    _sender->setup(host, _params.outputPort.get());

    ofxOscMessage hello;
    hello.setAddress("/hi");
    hello.addBoolArg(true);
    hello.addFloatArg(ofGetElapsedTimef());
    _sender->sendMessage(hello, false);
  }
  loadBindings(_appParams, _params.paramPrefix.get());
  sendParameterConfigs();
  sendAllParameters();
}

void OscController::handleClose(bool updateParams) {
  _sender.reset();
  _receiver.reset();
  _bindings.clear();
  if (updateParams) {
    _params.enabled.setWithoutEventNotifications(false);
  }
}

void OscController::loadBindings(::Params &params,
                                 const std::string& basePath) {
  for (auto param : params.getParamBases()) {
    if (!param->supportsOsc()) {
      continue;
    }
    if (param->isGroup()) {
      std::string subBasePath = basePath + param->getKey() + '/';
      loadBindings(dynamic_cast<::Params&>(*param), subBasePath);
    } else {
      std::shared_ptr<AbstractOscBinding> binding;
      try {
        binding = createBinding(*param,
                                basePath,
                                *this);
      } catch (std::invalid_argument ex) {
        ofLogWarning() << "Unable to create binding: " << ex.what();
        continue;
      }
      _bindings[binding->path()] = binding;
    }
  }
}

//void OscController::queueOutputMessage(const ofxOscMessage& message) {
//  if (_receiving) {
//    return;
//  }
//  _outputBundle.addMessage(message);
//}

void OscController::sendMessage(ofxOscMessage message) {
  if (_receiving) {
    return;
  }
  if (!_sender) {
    ofLogWarning() << "Unable to send osc message " << message.getAddress() << ", sender not initialized";
    return;
  }
  message.setRemoteEndpoint(_params.outputHost.get(),
                            _params.outputPort.get());
  _sender->sendMessage(message, false);
}

void OscController::sendAllParameters() {
  if (!_sender) {
    return;
  }
  _receiving = false;
  for (auto& entry : _bindings) {
    entry.second->sendValue();
  }
}

void OscController::sendParameterConfigs() {
  if (!_sender) {
    return;
  }
  _receiving = false;
  for (auto& entry : _bindings) {
    entry.second->sendConfig();
  }
}

void OscController::update() {
//  if (_sender && _outputBundle.getMessageCount() > 0) {
//    _sender->sendBundle(_outputBundle);
//  }
//  _outputBundle.clear();
  if (_receiver) {
    _receiving = true;
    ofxOscMessage message;
    while (_receiver->getNextMessage(message)) {
      handleMessage(message);
    }
    _receiving = false;
  }
}

void OscController::handleMessage(const ofxOscMessage &message) {
  auto iter = _bindings.find(message.getAddress());
  if (iter == _bindings.end()) {
    return;
  }
  iter->second->handleMessage(message);
}
