//
//  Display.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/01/09.
//
//

#ifndef Display_hpp
#define Display_hpp

#include "cocos2d.h"

namespace scene {
namespace layout {
namespace helper {

class Display {
  // member
public:
private:
  // function
public:
    static bool IsPortlate();
    
    static bool IsLandscape();

    static cocos2d::Size GetDrawingArea();

  private:
  };
        }
    }
    }
#endif /* Display_hpp */
