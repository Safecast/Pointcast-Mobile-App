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
        cocos2d::Size frame_size = lib::native::Util::getDisplaySize();
        
        float aspect = 0.0f;
        if (Display::IsPortlate()) {
          aspect = frame_size.height / frame_size.width;
        } else {
          aspect = frame_size.width / frame_size.height;
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
