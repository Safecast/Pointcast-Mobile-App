//
//  Display.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/01/09.
//
//
#include "Footer.hpp"

namespace scene {
namespace layout {
namespace helper {

void Footer::Visible(bool visible) {
  // change lowermenu visible
  cocos2d::EventCustom customEvent("footer_visible");
  auto value = cocos2d::Value(visible);
  customEvent.setUserData(&value);
  cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(
      &customEvent);
    
}
    
}
}
}
