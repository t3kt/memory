//
//  Params.cpp
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#include "Params.h"

template<>
void TParam<bool>::setNormalizedValue(float normVal) {
  ofParameter<bool>::set(normVal > 0);
}
