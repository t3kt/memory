//
//  OscController.cpp
//  memory
//
//  Created by tekt on 7/22/16.
//
//

#include "AppParameters.h"
#include "OscController.h"

class AbstractOscBinding {
public:
  AbstractOscBinding(std::string path, OscController& controller)
  : _path(path)
  , _controller(controller) { }

  virtual ~AbstractOscBinding() {}

  const std::string& path() const { return _path; }

  virtual void handleMessage(const ofxOscMessage& message) = 0;
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
    auto value = getMessageValue(message);
    _param.set(value);
  }

protected:
  void onParamChanged(T& value) {
    ofxOscMessage message;
    message.setAddress(_path);
    setMessageValue(message, value);
    sendMessage(message);
  }

  T getMessageValue(const ofxOscMessage& message) const;
  void setMessageValue(ofxOscMessage& message, const T& value) const;

  TParam<T>& _param;
};

template<>
float OscBinding<float>::getMessageValue(const ofxOscMessage &message) const {
  return message.getArgAsFloat(0);
}

template<>
int OscBinding<int>::getMessageValue(const ofxOscMessage &message) const {
  return message.getArgAsInt(0);
}

template<>
bool OscBinding<bool>::getMessageValue(const ofxOscMessage &message) const {
  return message.getArgAsBool(0);
}

template<>
void OscBinding<float>::setMessageValue(ofxOscMessage &message,
                                        const float &value) const {
  message.addFloatArg(value);
}

template<>
void OscBinding<int>::setMessageValue(ofxOscMessage &message,
                                      const int &value) const {
  message.addIntArg(value);
}

template<>
void OscBinding<bool>::setMessageValue(ofxOscMessage &message,
                                       const bool &value) const {
  message.addBoolArg(value);
}

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
    _sender->sendMessage(hello);
  }
  loadBindings(_appParams, _params.paramPrefix.get());
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

void OscController::update() {
//  if (_sender && _outputBundle.getMessageCount() > 0) {
//    _sender->sendBundle(_outputBundle);
//  }
//  _outputBundle.clear();
  if (_receiver) {
    _receiving = true;
    auto receiver = *_receiver;
    ofxOscMessage message;
    while (receiver.getNextMessage(message)) {
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
