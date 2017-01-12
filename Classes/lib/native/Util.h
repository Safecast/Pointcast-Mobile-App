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
  static void CloseIme();
  static void SetRotateEnable(bool flag);
  static cocos2d::Size GetDisplaySize();
  static void ChangeRotate(int type);

};
}
}
#endif /* Util_h */
