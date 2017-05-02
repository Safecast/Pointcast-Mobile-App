//
//  GoogleMapsManager.mm
//  pointcast
//
//  Created by Mitsuo Okada on 2015/12/08.
//
//

#import "AppController.h"
#include "lib/Util.hpp"
#include "lib/native/GoogleMapsManager.h"
#import <Foundation/Foundation.h>

USING_NS_CC;

namespace lib {

namespace native {

void GoogleMapsManager::attach(void) {
  CCLOG("GoogleMapsManager::start");
  AppController *app = [AppController getInstance];
  [app attachMap];
}

void GoogleMapsManager::detach(void) {
  AppController *app = [AppController getInstance];
  [app detachMap];
}

void GoogleMapsManager::attachMarker(std::string title, std::string snippet,
                                     double latitude, double longtitude,
                                     Color3B color, int zorder) {

  NSString *info_title = [NSString stringWithUTF8String:title.c_str()];
  NSString *info_snippet = [NSString stringWithUTF8String:snippet.c_str()];
  AppController *app = [AppController getInstance];
  UIColor *ui_color = [UIColor colorWithRed:color.r / 255.0f
                                      green:color.g / 255.0f
                                       blue:color.b / 255.0f
                                      alpha:1.0f];
  [app attachMarker:info_title
       info_snippet:info_snippet
           latitude:latitude
          longitude:longtitude
              color:ui_color
             zorder:zorder];
}
}
}
