//
//  Util.h
//  pointcast
//
//  Created by Mitsuo Okada on 2016/04/24.
//
//

#ifndef Util_h
#define Util_h

#include "cocos2d.h"
#include "lib/Util.hpp"

namespace lib {

namespace native {

class Util {
  // member
public:
private:
  // function
public:
  static void closeIme();
  static void setRotateEnable(bool flag);
};
}
}
#endif /* Util_h */
