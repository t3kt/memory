//
//  Status.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Status_h
#define Status_h

#include <vector>
#include <utility>
#include <ofxTextAlignTTF.h>

class StatusInfo {
public:
  using StatusLine = std::pair<std::string, std::string>;
  using StatusList = std::vector<StatusLine>;

  std::size_t registerLine(std::string name);

  void setValue(std::size_t index, std::string value);

  const StatusList& getLines() const { return _lines; }
private:
  StatusList _lines;
};

class StatusInfoProvider {
public:
  virtual const StatusInfo& getStatusInfo() const = 0;
};

class StatusInfoController {
public:
  void setup();

  void addProvider(StatusInfoProvider* provider) {
    _providers.push_back(provider);
  }

  void draw();
private:
  std::vector<StatusInfoProvider*> _providers;
  ofxTextAlignTTF _text;
};

#endif /* Status_h */
