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
        cocos2d::Size frame_size = lib::native::Util::GetDisplaySize();
        return frame_size.height > frame_size.width;
    }
    
    bool Display::IsLandscape()
    {
        return !Display::IsPortlate();
    }
    
    cocos2d::Size Display::GetDrawingArea()
    {
        cocos2d::Size display_size = lib::native::Util::GetDisplaySize();
        
        float aspect = 0.0f;
        if (Display::IsPortlate()) {
          aspect = display_size.height / display_size.width;
        } else {
          aspect = display_size.width / display_size.height;
        }
        cocos2d::Size size;
        if (aspect > 1.7f)
        {
            // 16:9
            if (Display::IsPortlate())
            {
                size = cocos2d::Size(640, 1136);
            } else {
                size = cocos2d::Size(1136, 640);
            }
        } else {
            // 4:3
            // @todo change aspect
            if (Display::IsPortlate())
            {
                size = cocos2d::Size(640, 1136);
            } else {
                size = cocos2d::Size(1136, 640);
            }
        }
        return size;
        
    }
}
}
}
