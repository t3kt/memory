//
//  OscDevice.h
//  memory
//
//  Created by tekt on 7/21/16.
//
//

#ifndef OscDevice_h
#define OscDevice_h

#include <memory>
#include <ofxOsc.h>
#include <string>
#include "ControlCommon.h"
#include "Params.h"

class OscDevice {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(inputEnabled
          .setKey("inputEnabled")
          .setName("Input Enabled"));
      add(inputPort
          .setKey("inputPort")
          .setName("Input Port")
          .setValueAndDefault(9001)
          .setRange(0, 65535));
      add(inputEnabled
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
    }
    TParam<std::string> outputHost;
    TParam<int> outputPort;
    TParam<int> inputPort;
    TParam<bool> inputEnabled;
    TParam<bool> outputEnabled;
  };

  OscDevice(Params& params);
  virtual ~OscDevice() {
    handleClose(false);
  }

  const ControlDeviceId& id() const { return _id; }

private:
  void handleOpen();
  void handleClose(bool updateParams);

  const ControlDeviceId _id;
  Params& _params;
  std::shared_ptr<ofxOscSender> _sender;
};

#endif /* OscDevice_h */
