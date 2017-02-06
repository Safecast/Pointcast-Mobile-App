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
    enum OrientationType
    {
        UIDeviceOrientationUnknown,
        UIDeviceOrientationPortrait,
        UIDeviceOrientationPortraitUpsideDown,
        UIDeviceOrientationLandscapeRight,
        UIDeviceOrientationLandscapeLeft,
    };
    
    
private:
  // function
public:
  static void closeIme();
  static void setRotateEnable(bool flag);
  static cocos2d::Size getDisplaySize();
  static void changeRotate(int type);
  static void showSortPicker();
  static void showSearchWordInputText();
};
}
}
#endif /* Util_h */
