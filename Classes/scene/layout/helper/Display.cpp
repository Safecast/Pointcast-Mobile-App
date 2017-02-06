//
//  Display.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/01/09.
//
//
#include "Display.hpp"

#include "lib/native/Util.h"

namespace scene {
namespace layout {
namespace helper {
    
    bool Display::IsPortlate()
    {
        cocos2d::Size frame_size = lib::native::Util::getDisplaySize();
        return frame_size.height > frame_size.width;
    }
    
    bool Display::IsLandscape()
    {
        return !Display::IsPortlate();
    }
    
    cocos2d::Size Display::GetDrawingArea()
    {
        cocos2d::Size display_size = lib::native::Util::getDisplaySize();
        
        if (Display::IsPortlate()) {
            return cocos2d::Size(display_size.width, display_size.height);
        } else {
            return cocos2d::Size(display_size.width, display_size.height);
        }
    }
}
}
}
