//
//  ParamsGui.cpp
//  memory
//
//  Created by tekt on 7/19/16.
//
//

#include <ofLog.h>
#include <stdexcept>
#include "ParamsGui.h"

std::shared_ptr<ParamGui> createParamGuiControl(TParamBase& param) {
  const auto& type = param.getTypeInfo();
  if (type == typeid(bool)) {
    return std::make_shared<ParamGuiControl<bool>>(static_cast<TParam<bool>&>(param));
  } else {
    ofLogWarning() << "Unsupported param type: " << type.name();
    return std::shared_ptr<ParamGui>();
  }
}

void ParamsGui::addControlsForParams() {
  for (auto param : _params.paramBases()) {
    auto ctrl = createParamGuiControl(*param);
    if (ctrl) {
      ctrl->addToParent(&_root);
      _controls.push_back(ctrl);
    }
  }
}

void ParamsGui::addToParent(ofxControlWidget* parent) {
  _root.setName(_params.getName());
  parent->addWidget(&_root);
  build();
}

void ParamsGui::build() {
  addControlsForParams();
}
